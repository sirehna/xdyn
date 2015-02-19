#include "WebSocketObserver.hpp"
#include "WebSocketObserverTest.hpp"

#include <iostream>
#include <unistd.h> //usleep

#include <ssc/macros.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::thread;

// pull out the type of messages sent by our config
typedef WSServer::message_ptr message_ptr;

#define MESSAGE_SENT "First message"

// Define a callback to handle incoming messages
void on_message(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg);
void on_message(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
    ASSERT_EQ(MESSAGE_SENT, msg->get_payload());
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

void createServerEcho(WSServer& echo_server);
void createServerEcho(WSServer& echo_server)
{
    echo_server.set_reuse_addr(true);
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    echo_server.init_asio();
    // Register our message handler
    echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));
    // Listen on port 9002
    echo_server.listen("127.0.0.1","9002");
    // Start the server accept loop
    echo_server.start_accept();
    // Start the ASIO io_service run loop
    echo_server.run();
}

int connectToServer(WebSocketEndpoint& endpoint);
int connectToServer(WebSocketEndpoint& endpoint)
{
    usleep(10000);
    std::cout << "Start creating observer" << std::endl<<std::flush;
    size_t k=0;
    int id = endpoint.connect("ws://localhost:9002");
    while(true)
    {
        connection_metadata::ptr metadata = endpoint.get_metadata(id);
        k++;
        if (k>100)
        {
            std::stringstream ss;
            ss << "Time out" <<id<<std::endl;
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
            ss << "Connection failed" <<id<<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
            break;
        }
        usleep(100000);
    }
    COUT(k);
    connection_metadata::ptr metadata = endpoint.get_metadata(id);
    if (metadata)
    {
        std::cout << *metadata << std::endl<<std::flush;
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown connection id : " <<id<<std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
    }
    return id;
}

WebSocketObserverTest::WebSocketObserverTest():server(),threadServer(createServerEcho, std::ref(server))
{
}

WebSocketObserverTest::~WebSocketObserverTest()
{
    server.stop();
    threadServer.join();
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_connect_a_web_socket_server)
{
    WebSocketEndpoint endpoint;
    const int id = connectToServer(endpoint);
    endpoint.send(id,MESSAGE_SENT);
}
