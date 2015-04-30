/*
 * WebSocketServer.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include <ssc/macros.hpp>

#include "WebSocketServer.hpp"
#include "WebSocketEndpoint.hpp"
#include "WebSocketObserverException.hpp"

WebSocketServer::WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> m):
        server(),
        threadServer(createServerEcho,std::ref(server),m)
{
}

WebSocketServer::~WebSocketServer()
{
    std::cout<<"Calling WebSocketServer::~WebSocketServer"<<std::endl<<std::flush;
    server.stop();
    threadServer.join();
}

void createServerEcho(WSServer& echo_server, std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> f)
{
    echo_server.set_reuse_addr(true);
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    echo_server.init_asio();
    // Register our message handler
    echo_server.set_message_handler(bind(f,&echo_server,::_1,::_2));
    // Listen on port
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(STR(ADDRESS)), PORT);
    echo_server.listen(ep);
    // Start the server accept loop
    echo_server.start_accept();
    // Start the ASIO io_service run loop
    echo_server.run();
}

int connectToServer(WebSocketEndpoint& endpoint)
{
    usleep(10000);
    size_t k=0;
    const std::string connectAddress(std::string("ws://")+std::string(STR(ADDRESS))+std::string(":")+std::string(STR(PORT)));
    std::cout << "Start creating observer :" << connectAddress<<std::endl<<std::flush;
    endpoint.connect(connectAddress);
    while(true)
    {
        connection_metadata::ptr metadata = endpoint.get_metadata(endpoint.get_current_id());
        k++;
        if (k>100)
        {
            std::stringstream ss;
            ss << "Time out: " << std::boolalpha << endpoint.good() << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        }
        std::cout<<metadata->get_status()<<std::endl;
        if (metadata->get_status()=="Open")
        {
            break;
        }
        else if (metadata->get_status()=="Failed")
        {
            std::stringstream ss;
            ss << "Connection failed" << endpoint.good() <<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
            break;
        }
        usleep(100000);
    }
    COUT(k);
    connection_metadata::ptr metadata = endpoint.get_metadata(endpoint.get_current_id());
    if (metadata)
    {
        std::cout << *metadata << std::endl<<std::flush;
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown connection id : " <<endpoint.get_current_id()<<std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
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
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
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
