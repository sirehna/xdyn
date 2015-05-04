/*
 * WebSocketServer.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include "WebSocketServer.hpp"
#include "WebSocketException.hpp"

typedef std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> InternalMessageHandler;

using std::placeholders::_1;
using std::placeholders::_2;

void create_server(WSServer& server, const std::string& address, const short unsigned int port, const InternalMessageHandler& message_handler);

InternalMessageHandler get_lambda(const TR1(shared_ptr)<WebSocketClient>& socket, const MessageHandler& message_handler);
InternalMessageHandler get_lambda(const TR1(shared_ptr)<WebSocketClient>& socket, const MessageHandler& message_handler)
{
    return [message_handler,socket](WSServer* server, const websocketpp::connection_hdl& handle, const message_ptr& mes )
                                                                {Message msg;
                                                                 msg.handle = handle;
                                                                 msg.message = mes;
                                                                 msg.server = server;
                                                                 msg.socket = socket;
                                                                 message_handler(msg);};
}

WebSocketServer::WebSocketServer(const MessageHandler& message_handler, const std::string& address, const short unsigned int port):
        server(),
        server_thread(),
        socket(new WebSocketClient())
{
    const auto lambda = get_lambda(socket,message_handler);
    server_thread = std::thread(create_server, std::ref(server), address, port, lambda);
    *socket = WebSocketClient(std::string("ws://") + address,port);
}

WebSocketServer::~WebSocketServer()
{
    server.stop();
    server_thread.join();
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
