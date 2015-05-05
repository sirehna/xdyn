#include "string2vector.hpp"
#include "WebSocketObserverTest.hpp"
#include "WebSocketException.hpp"
#include "WebSocketClient.hpp"
#include "WebSocketServer.hpp"

#include <iostream>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    9002

struct ListOfStringMessages : public MessageHandler
{
    ListOfStringMessages() : messages()
    {
    }
    void operator()(const WebSocketMessage& msg)
    {
        messages.push_back(msg.get_payload());
    }

    std::vector<std::string> messages;
};

struct ListOfDoubleMessages : public MessageHandler
{
    ListOfDoubleMessages() : messages()
    {
    }
    void operator()(const WebSocketMessage& msg)
    {
        const auto v = msg.get_binary_payload<double>();
        messages.insert(messages.end(), v.begin(), v.end());
    }

    std::vector<double> messages;
};

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_connect_a_web_socket_server)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(ADDRESS, WEBSOCKET_PORT));
    {
        WebSocketClient endpoint(WEBSOCKET_ADDRESS, WEBSOCKET_PORT);
        ASSERT_TRUE(endpoint.good());
    }
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_send_a_string)
{
    const std::string s1 = a.random<std::string>();
    const std::string s2 = a.random<std::string>();
    const std::string s3 = a.random<std::string>();
    ListOfStringMessages message_handler;
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(message_handler, ADDRESS, WEBSOCKET_PORT));
    { // Creating a scope ensures that the message is actually sent at the end of the scope
        WebSocketClient endpoint(WEBSOCKET_ADDRESS, WEBSOCKET_PORT);
        endpoint.send_text(s1);
        endpoint.send_text(s2);
        endpoint.send_text(s3);
    }
    ASSERT_EQ(3, message_handler.messages.size());
    ASSERT_EQ(s1, message_handler.messages.at(0));
    ASSERT_EQ(s2, message_handler.messages.at(1));
    ASSERT_EQ(s3, message_handler.messages.at(2));
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_send_a_vector_doubles)
{
    ListOfDoubleMessages message_handler;
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(message_handler, ADDRESS, WEBSOCKET_PORT));
    {
        std::vector<double> v(3,0.0);
        v[0]=1.0;v[1]=2.0;v[2]=3.0;
        WebSocketClient endpoint(WEBSOCKET_ADDRESS, WEBSOCKET_PORT);
        endpoint.send_binary(v);
    }
    ASSERT_EQ(3, message_handler.messages.size());
    ASSERT_DOUBLE_EQ(1, message_handler.messages.at(0));
    ASSERT_DOUBLE_EQ(2, message_handler.messages.at(1));
    ASSERT_DOUBLE_EQ(3, message_handler.messages.at(2));
}
