/*
 * WebSocketServer.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include "WebSocketServer.hpp"
#include "WebSocketException.hpp"
#include "WebSocketMessageImpl.hpp"

using namespace ssc::websocket;

struct Server::Impl
{
    Impl() :
        server(),
        server_thread()
    {}

    WSServer server;
    std::thread server_thread; // Thread in which the server runs
};



typedef std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> InternalMessageHandler;

using std::placeholders::_1;
using std::placeholders::_2;

void create_server(WSServer& server, const std::string& address, const short unsigned int port, const InternalMessageHandler& message_handler);

InternalMessageHandler get_lambda(MessageHandler& message_handler);
InternalMessageHandler get_lambda(MessageHandler& message_handler)
{
    return [&message_handler](WSServer* server, const websocketpp::connection_hdl& handle, const message_ptr& mes )
                                                                {
                                                                 WebSocketMessageImpl pimpl;
                                                                 pimpl.handle = handle;
                                                                 pimpl.message = mes;
                                                                 pimpl.server = server;
                                                                 message_handler(Message(pimpl));};
}

Server::Server(MessageHandler& message_handler, const std::string& address, const short unsigned int port):
        pimpl(new Impl())
{
    pimpl->server_thread = std::thread(create_server, std::ref(pimpl->server), address, port, get_lambda(message_handler));
}

struct DoNothing : public MessageHandler
{
    void operator()(const Message&){}
};

Server::Server(const std::string& address, const short unsigned int port) : pimpl(new Impl())
{
    DoNothing message_handler;
    pimpl->server_thread = std::thread(create_server, std::ref(pimpl->server), address, port, get_lambda(message_handler));
}


Server::~Server()
{
    pimpl->server.stop();
    pimpl->server_thread.join();
}


void create_server(WSServer& server, const std::string& address, const short unsigned int port, const InternalMessageHandler& message_handler)
{
    server.set_reuse_addr(true);
    // Set logging settings
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    server.init_asio();
    // Register our message handler
    server.set_message_handler(std::bind(message_handler,&server,::_1,::_2));
    // Listen on port
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(address), port);
    server.listen(ep);
    // Start the server accept loop
    server.start_accept();
    // Start the ASIO io_service run loop
    server.run();
}
