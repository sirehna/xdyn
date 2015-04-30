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

#define ADDRESS 127.0.0.1
#define PORT 9002
#define MESSAGE_SENT "First message"

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

class WebSocketEndpoint;
typedef std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> MessageHandler;

class WebSocketServer
{
    public:
        WebSocketServer(const MessageHandler& message_handler);
        ~WebSocketServer();

    protected:
        WSServer server;
        websocketpp::lib::thread threadServer;

    private:
        WebSocketServer();
};


void create_server_echo(WSServer& echo_server, const MessageHandler& message_handler, const std::string& address, const short unsigned int port);
void on_message_string(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg);


#endif  /* WEBSOCKETSERVER_HPP_ */
