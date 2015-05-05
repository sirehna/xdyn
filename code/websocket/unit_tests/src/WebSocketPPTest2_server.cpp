#include <iostream>

// Test Environment:
// server, no TLS, no locks, iostream based transport
#include <websocketpp/config/core.hpp>
#include <websocketpp/server.hpp>

#include "gtest/gtest.h"
class WebSocketPPTest2_server : public ::testing::Test
{
    protected:
        WebSocketPPTest2_server(){};
        virtual ~WebSocketPPTest2_server(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
};

typedef websocketpp::server<websocketpp::config::core> server;
typedef websocketpp::config::core::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

/* Run server and return output test rig */
std::string run_server_test(server& s, std::string input);
std::string run_server_test(server& s, std::string input)
{
    server::connection_ptr con;
    std::stringstream output;

    s.register_ostream(&output);
    s.clear_access_channels(websocketpp::log::alevel::all);
    s.clear_error_channels(websocketpp::log::elevel::all);

    con = s.get_connection();
    con->start();

    std::stringstream channel;

    channel << input;
    channel >> *con;

    return output.str();
}

/* handler library*/
void echo_func(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
void echo_func(server* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    s->send(hdl, msg->get_payload(), msg->get_opcode());
}

bool validate_func_subprotocol(server* s, std::string* out, std::string accept,
    websocketpp::connection_hdl hdl);
bool validate_func_subprotocol(server* s, std::string* out, std::string accept,
    websocketpp::connection_hdl hdl)
{
    server::connection_ptr con = s->get_con_from_hdl(hdl);

    std::stringstream o;

    const std::vector<std::string> & protocols = con->get_requested_subprotocols();
    std::vector<std::string>::const_iterator it;

    for (it = protocols.begin(); it != protocols.end(); ++it) {
        o << *it << ",";
    }

    *out = o.str();

    if (accept != "") {
        con->select_subprotocol(accept);
    }

    return true;
}

void open_func_subprotocol(server* s, std::string* out, websocketpp::connection_hdl hdl);
void open_func_subprotocol(server* s, std::string* out, websocketpp::connection_hdl hdl)
{
    server::connection_ptr con = s->get_con_from_hdl(hdl);

    *out = con->get_subprotocol();
}

/* Tests */
TEST_F(WebSocketPPTest2_server, basic_websocket_request ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\n\r\n";
    std::string output = "HTTP/1.1 101 Switching Protocols\r\nConnection: upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\nServer: test\r\nUpgrade: websocket\r\n\r\n";

    server s;
    s.set_user_agent("test");

    ASSERT_EQ(run_server_test(s,input), output);
}

TEST_F(WebSocketPPTest2_server,  invalid_websocket_version ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: a\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\n\r\n";
    std::string output = "HTTP/1.1 400 Bad Request\r\nServer: test\r\n\r\n";

    server s;
    s.set_user_agent("test");
    //s.set_message_handler(bind(&echo_func,&s,::_1,::_2));

    ASSERT_EQ(run_server_test(s,input), output);
}

TEST_F(WebSocketPPTest2_server,  unimplemented_websocket_version ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 14\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\n\r\n";

    std::string output = "HTTP/1.1 400 Bad Request\r\nSec-WebSocket-Version: 0,7,8,13\r\nServer: test\r\n\r\n";

    server s;
    s.set_user_agent("test");

    ASSERT_EQ(run_server_test(s,input), output);
}

TEST_F(WebSocketPPTest2_server,  list_subprotocol_empty ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\nSec-WebSocket-Protocol: foo\r\n\r\n";

    std::string output = "HTTP/1.1 101 Switching Protocols\r\nConnection: upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\nServer: test\r\nUpgrade: websocket\r\n\r\n";

    std::string subprotocol;

    server s;
    s.set_user_agent("test");
    s.set_open_handler(bind(&open_func_subprotocol,&s,&subprotocol,::_1));

    ASSERT_EQ(run_server_test(s,input), output);
    ASSERT_EQ(subprotocol, "");
}

TEST_F(WebSocketPPTest2_server,  list_subprotocol_one ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\nSec-WebSocket-Protocol: foo\r\n\r\n";

    std::string output = "HTTP/1.1 101 Switching Protocols\r\nConnection: upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\nServer: test\r\nUpgrade: websocket\r\n\r\n";

    std::string validate;
    std::string open;

    server s;
    s.set_user_agent("test");
    s.set_validate_handler(bind(&validate_func_subprotocol,&s,&validate,"",::_1));
    s.set_open_handler(bind(&open_func_subprotocol,&s,&open,::_1));

    ASSERT_EQ(run_server_test(s,input), output);
    ASSERT_EQ(validate, "foo,");
    ASSERT_EQ(open, "");
}

TEST_F(WebSocketPPTest2_server,  accept_subprotocol_one ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\nSec-WebSocket-Protocol: foo\r\n\r\n";

    std::string output = "HTTP/1.1 101 Switching Protocols\r\nConnection: upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\nSec-WebSocket-Protocol: foo\r\nServer: test\r\nUpgrade: websocket\r\n\r\n";

    std::string validate;
    std::string open;

    server s;
    s.set_user_agent("test");
    s.set_validate_handler(bind(&validate_func_subprotocol,&s,&validate,"foo",::_1));
    s.set_open_handler(bind(&open_func_subprotocol,&s,&open,::_1));

    ASSERT_EQ(run_server_test(s,input), output);
    ASSERT_EQ(validate, "foo,");
    ASSERT_EQ(open, "foo");
}

TEST_F(WebSocketPPTest2_server,  accept_subprotocol_invalid ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\nSec-WebSocket-Protocol: foo\r\n\r\n";

    std::string output = "HTTP/1.1 101 Switching Protocols\r\nConnection: upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\nSec-WebSocket-Protocol: foo\r\nServer: test\r\nUpgrade: websocket\r\n\r\n";

    std::string validate;
    std::string open;

    server s;
    s.set_user_agent("test");
    s.set_validate_handler(bind(&validate_func_subprotocol,&s,&validate,"foo2",::_1));
    s.set_open_handler(bind(&open_func_subprotocol,&s,&open,::_1));

    std::string o;

    ASSERT_THROW(o = run_server_test(s,input), websocketpp::exception);
}

TEST_F(WebSocketPPTest2_server,  accept_subprotocol_two ) {
    std::string input = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: upgrade\r\nUpgrade: websocket\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://www.example.com\r\nSec-WebSocket-Protocol: foo, bar\r\n\r\n";

    std::string output = "HTTP/1.1 101 Switching Protocols\r\nConnection: upgrade\r\nSec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\nSec-WebSocket-Protocol: bar\r\nServer: test\r\nUpgrade: websocket\r\n\r\n";

    std::string validate;
    std::string open;

    server s;
    s.set_user_agent("test");
    s.set_validate_handler(bind(&validate_func_subprotocol,&s,&validate,"bar",::_1));
    s.set_open_handler(bind(&open_func_subprotocol,&s,&open,::_1));

    ASSERT_EQ(run_server_test(s,input), output);
    ASSERT_EQ(validate, "foo,bar,");
    ASSERT_EQ(open, "bar");
}
