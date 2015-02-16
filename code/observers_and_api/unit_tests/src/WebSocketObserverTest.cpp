#include "WebSocketObserver.hpp"
#include "WebSocketObserverTest.hpp"

#include <iostream>

#include <boost/date_time/time_clock.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

#include <ssc/macros.hpp>

typedef websocketpp::server<websocketpp::config::asio> WSServer;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::thread;

// pull out the type of messages sent by our config
typedef WSServer::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg);
void on_message(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
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

WebSocketObserverTest::WebSocketObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

WSServer* foo();
WSServer* foo()
{
    WSServer* echo_server = new WSServer();
    // Set logging settings
    echo_server->set_access_channels(websocketpp::log::alevel::all);
    echo_server->clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    echo_server->init_asio();
    // Register our message handler
    echo_server->set_message_handler(bind(&on_message,echo_server,::_1,::_2));
    // Listen on port 9002
    echo_server->listen(9002);
    // Start the server accept loop
    echo_server->start_accept();
    std::cout << "Start running" << std::endl<<std::flush;
    // Start the ASIO io_service run loop
    //echo_server->run();
    //echo_server->run_one();
    //thread t(bind(&WSServer::run,&echo_server));
    //t.detach();
    //echo_server->poll();
    return echo_server;
}

#include <thread>
#include <chrono>
TEST_F(WebSocketObserverTest, should_be_able_to_connect_a_web_socket_server)
{
    // Create a server endpoint
    // WSServer echo_server;
    WSServer * echo_server = foo();
//    try {
        /*
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
        // Initialize ASIO
        echo_server.init_asio();
        // Register our message handler
        echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));
        // Listen on port 9002
        echo_server.listen(9002);
        // Start the server accept loop
        echo_server.start_accept();
        std::cout << "Start running" << std::endl<<std::flush;
        // Start the ASIO io_service run loop
        //echo_server.run();
        //echo_server.run_one();
        //thread t(bind(&WSServer::run,&echo_server));
        //t.detach();
        echo_server.poll();
        //std::this_thread::sleep_for (std::chrono::seconds(1));
        */
        echo_server->poll();
        {
            //std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Start creating observer" << std::endl<<std::flush;
            WebSocketEndpoint endpoint;
            int id=endpoint.connect("ws://localhost:9002");
            COUT(id);
            endpoint.send(id,"dfsqfdsqg");

            //WebSocketObserver ss("ws://localhost:9002",std::vector<std::string>(1,""));
            //std::cout << "Display state" << std::endl<<std::flush;
            //std::cout<<ss;
            //std::cout << "Display state" << std::endl<<std::flush;
            //std::cout<<ss;
            //std::cout << "Send message" << std::endl<<std::flush;
            //ss.send("toto");
            //std::cout << "Display state" << std::endl<<std::flush;
            //std::cout<<ss;
            ////std::cout<<ss;
        }
        std::cout << "Stop server" << std::endl<<std::flush;
        echo_server->stop();
        echo_server->stop_perpetual();
        delete echo_server;
//    }
//    catch (websocketpp::exception const & e)
//    {
//        std::cout << e.what() << std::endl<<std::flush;
//        delete echo_server;
//    }
//    catch (...)
//    {
//        std::cout << "other exception" << std::endl<<std::flush;
//        delete echo_server;
//    }
}
