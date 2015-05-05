#include "string2vector.hpp"
#include "WebSocketObserverTest.hpp"
#include "WebSocketException.hpp"
#include "WebSocketClient.hpp"
#include "WebSocketServer.hpp"

#include <iostream>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#define MESSAGE_SENT      "First message"
#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    9002

void on_message_string(const WebSocketMessage& msg);
void on_message_string(const WebSocketMessage& msg)
{
    if (MESSAGE_SENT != msg.get_payload())
    {
        std::stringstream ss;
        ss << "Message sent does not match payload: MESSAGE_SENT=" << MESSAGE_SENT << " but payload=" << msg.get_payload();
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }
    msg.send_text(msg.get_payload());
}

void on_message_vector(const WebSocketMessage& msg);
void on_message_vector(const WebSocketMessage& msg)
{
    ASSERT_EQ(websocketpp::frame::opcode::binary, msg.message->get_opcode());
    const std::vector<double> payload = msg.get_binary_payload<double>();
    ASSERT_EQ(3,payload.size());
    ASSERT_EQ(1.0,payload[0]);
    ASSERT_EQ(2.0,payload[1]);
    ASSERT_EQ(3.0,payload[2]);
    msg.send_binary(payload);
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_connect_a_web_socket_server)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(on_message_string, ADDRESS, WEBSOCKET_PORT));
    {
        WebSocketClient endpoint(WEBSOCKET_ADDRESS, WEBSOCKET_PORT);
        ASSERT_TRUE(endpoint.good());
    }
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_send_a_string)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(on_message_string, ADDRESS, WEBSOCKET_PORT));
    {
        WebSocketClient endpoint(WEBSOCKET_ADDRESS, WEBSOCKET_PORT);
        endpoint.send_text(MESSAGE_SENT);
    }
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_send_a_vector_doubles)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(on_message_vector, ADDRESS, WEBSOCKET_PORT));
    {
        std::vector<double> v(3,0.0);
        v[0]=1.0;v[1]=2.0;v[2]=3.0;
        WebSocketClient endpoint(WEBSOCKET_ADDRESS, WEBSOCKET_PORT);
        endpoint.send_binary(v);
    }
}

TEST_F(WebSocketObserverTest, should_be_able_to_convert_vector_of_doubles_to_string_and_back)
{
    const size_t n=3;
    std::vector<double> v(n,0.0);
    v[0]=1.0;v[1]=2.0;v[2]=3.0;
    const std::string s = convert_vector_to_string(v);
    ASSERT_EQ(3*8,s.size());
    std::vector<double> v2=convert_string_to_vector<double>(s);
    ASSERT_EQ(n,v2.size());
    for (size_t i=0;i<n;++i)
    {
        ASSERT_EQ(v[i],v2[i]);
    }
}
