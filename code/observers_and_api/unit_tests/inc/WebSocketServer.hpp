/*
 * WebSocketServer.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETSERVER_HPP_
#define WEBSOCKETSERVER_HPP_

#include <functional>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/thread.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::thread;

#ifndef STR
#define STR(s) STR_(s)
#endif

#ifndef STR_
#define STR_(s) #s
#endif

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;
typedef std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> MessageHandler;

class WebSocketServer
{
    public:
        WebSocketServer(const MessageHandler& message_handler, const std::string& address, const short unsigned int port);
        ~WebSocketServer();

    protected:
        WSServer server;
        websocketpp::lib::thread server_thread; // Thread in which the server runs

    private:
        void create_echo_server(WSServer& echo_server, const MessageHandler& message_handler, const std::string& address, const short unsigned int port);
        WebSocketServer();
};


#endif  /* WEBSOCKETSERVER_HPP_ */
