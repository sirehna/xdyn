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

int connect_to_server(WebSocketEndpoint& endpoint, const std::string& address)
{
    usleep(10000);
    size_t k=0;
    std::cout << "Starting server at: " << address<<std::endl<<std::flush;
    endpoint.connect(address);
    while(true)
    {
        connection_metadata::ptr metadata = endpoint.get_metadata(endpoint.get_current_id());
        k++;
        if (k>100)
        {
            std::stringstream ss;
            ss << "Time out when retrieving metadata from the endpoint" << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
        }
        std::cout<<metadata->get_status()<<std::endl;
        if (metadata->get_status()=="Open")
        {
            break;
        }
        else if (metadata->get_status()=="Failed")
        {
            std::stringstream ss;
            ss << "Connection failed" <<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
            break;
        }
        usleep(100000);
    }
    connection_metadata::ptr metadata = endpoint.get_metadata(endpoint.get_current_id());
    if (metadata)
    {
        std::cout << *metadata << std::endl<<std::flush;
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown connection id : " <<endpoint.get_current_id()<<std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }
    return endpoint.get_current_id();
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
