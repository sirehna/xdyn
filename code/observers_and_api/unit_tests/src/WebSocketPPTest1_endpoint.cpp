#include <iostream>
#include <sstream>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include "gtest/gtest.h"
class WebSocketPPTest1_endpoint : public ::testing::Test
{
    protected:
        WebSocketPPTest1_endpoint(){};
        virtual ~WebSocketPPTest1_endpoint(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
};

TEST_F(WebSocketPPTest1_endpoint,  construct_server_iostream ) {
    websocketpp::server<websocketpp::config::core> s;
}

TEST_F(WebSocketPPTest1_endpoint,  construct_server_asio_plain ) {
    websocketpp::server<websocketpp::config::asio> s;
}

//TEST_F(WebSocketPPTest1_endpoint,  construct_server_asio_tls ) {
//    websocketpp::server<websocketpp::config::asio_tls> s;
//    COUT(s);
//}

TEST_F(WebSocketPPTest1_endpoint,  initialize_server_asio ) {
    websocketpp::server<websocketpp::config::asio> s;
    s.init_asio();
}

//TEST_F(WebSocketPPTest1_endpoint,  initialize_server_asio_external ) {
//    websocketpp::server<websocketpp::config::asio> s;
//    boost::asio::io_service ios;
//    s.init_asio(&ios);
//}

struct endpoint_extension {
    endpoint_extension() : extension_value(5) {}

    int extension_method() {
        return extension_value;
    }

    bool is_server() const {
        return false;
    }

    int extension_value;
};

struct stub_config : public websocketpp::config::core {
    typedef core::concurrency_type concurrency_type;

    typedef core::request_type request_type;
    typedef core::response_type response_type;

    typedef core::message_type message_type;
    typedef core::con_msg_manager_type con_msg_manager_type;
    typedef core::endpoint_msg_manager_type endpoint_msg_manager_type;

    typedef core::alog_type alog_type;
    typedef core::elog_type elog_type;

    typedef core::rng_type rng_type;

    typedef core::transport_type transport_type;

    typedef endpoint_extension endpoint_base;
};

TEST_F(WebSocketPPTest1_endpoint,  endpoint_extensions ) {
    websocketpp::server<stub_config> s;

    ASSERT_EQ( s.extension_value, 5 );
    ASSERT_EQ( s.extension_method(), 5 );
    ASSERT_TRUE( s.is_server() );
}

TEST_F(WebSocketPPTest1_endpoint,  listen_after_listen_failure )
{
    using websocketpp::transport::asio::error::make_error_code;
    using websocketpp::transport::asio::error::pass_through;

    websocketpp::server<websocketpp::config::asio> server1;
    websocketpp::server<websocketpp::config::asio> server2;

    websocketpp::lib::error_code ec;

    server1.init_asio();
    server2.init_asio();

    boost::asio::ip::tcp::endpoint ep1(boost::asio::ip::address::from_string("127.0.0.1"), 12345);
    boost::asio::ip::tcp::endpoint ep2(boost::asio::ip::address::from_string("127.0.0.1"), 23456);

    server1.listen(ep1, ec);
    ASSERT_TRUE(!ec);

    server2.listen(ep1, ec);
    ASSERT_EQ(ec, make_error_code(pass_through));

    server2.listen(ep2, ec);
    ASSERT_TRUE(!ec);
}
