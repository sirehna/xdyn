/*
 * WebSocketServer.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include "WebSocketServer.hpp"
#include "WebSocketEndpoint.hpp"
#include "WebSocketObserverException.hpp"

WebSocketServer::WebSocketServer(const MessageHandler& message_handler):
        server(),
        threadServer(create_server_echo, std::ref(server), message_handler, STR(ADDRESS), PORT)
{
}

WebSocketServer::~WebSocketServer()
{
    server.stop();
    threadServer.join();
}

void create_server_echo(WSServer& echo_server, const MessageHandler& message_handler, const std::string& address, const short unsigned int port)
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

void on_message_string(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
    if (MESSAGE_SENT != msg->get_payload())
    {
        std::stringstream ss;
        ss << "Message sent does not match payload: MESSAGE_SENT=" << MESSAGE_SENT << " but payload=" << msg->get_payload();
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }
    try
    {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    }
    catch (const websocketpp::lib::error_code& e)
    {
        std::cout << "Echo failed because: " << e
                  << "(" << e.message() << ")" << std::endl;
    }
}
