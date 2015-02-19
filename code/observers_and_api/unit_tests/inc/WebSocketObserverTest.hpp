#ifndef WEBSOCKETOBSERVERTEST_HPP_
#define WEBSOCKETOBSERVERTEST_HPP_

#include "gtest/gtest.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
typedef websocketpp::server<websocketpp::config::asio> WSServer;

class WebSocketObserverTest : public ::testing::Test
{
    protected:
        WebSocketObserverTest();
        virtual ~WebSocketObserverTest();
        virtual void SetUp(){};
        virtual void TearDown(){};
        WSServer server;
        websocketpp::lib::thread threadServer;
};

#endif
