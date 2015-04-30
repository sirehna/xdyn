/*
 * WebSocketServer.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include "WebSocketServer.hpp"
#include "WebSocketException.hpp"

typedef std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> InternalMessageHandler;

void create_echo_server(WSServer& echo_server, const std::string& address, const short unsigned int port, const InternalMessageHandler& message_handler);

InternalMessageHandler get_lambda(const MessageHandler& message_handler);
InternalMessageHandler get_lambda(const MessageHandler& message_handler)
{
    return [message_handler](WSServer* server, const websocketpp::connection_hdl& handle, const message_ptr& mes )
                                                                {Message msg;
                                                                 msg.handle = handle;
                                                                 msg.message = mes;
                                                                 msg.server = server;
                                                                 message_handler(msg);};
}

WebSocketServer::WebSocketServer(const MessageHandler& message_handler, const std::string& address, const short unsigned int port):
        server(),
        server_thread(create_echo_server, std::ref(server), address, port, get_lambda(message_handler))
{
}

WebSocketServer::~WebSocketServer()
{
    server.stop();
    server_thread.join();
}


void create_echo_server(WSServer& echo_server, const std::string& address, const short unsigned int port, const InternalMessageHandler& message_handler)
{
    echo_server.set_reuse_addr(true);
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    echo_server.init_asio();
    // Register our message handler
    echo_server.set_message_handler(bind(message_handler,&echo_server,::_1,::_2));
    // Listen on port
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(address), port);
    echo_server.listen(ep);
    // Start the server accept loop
    echo_server.start_accept();
    // Start the ASIO io_service run loop
    echo_server.run();
}
