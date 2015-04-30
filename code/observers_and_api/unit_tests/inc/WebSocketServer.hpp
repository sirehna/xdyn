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

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

struct Message
{
    Message() : server(), handle(), message() {}
    ~Message() {}
    Message(const Message& rhs) : server(rhs.server), handle(rhs.handle), message(rhs.message) {}
    Message& operator=(const Message& rhs)
    {
        server = rhs.server;
        handle = rhs.handle;
        message = rhs.message;
        return *this;
    }
    WSServer* server;
    websocketpp::connection_hdl handle;
    message_ptr message;
};

typedef std::function<void(const Message&)> MessageHandler;

class WebSocketServer
{
    public:
        WebSocketServer(const MessageHandler& message_handler, const std::string& address, const short unsigned int port);
        ~WebSocketServer();

    protected:
        WSServer server;
        websocketpp::lib::thread server_thread; // Thread in which the server runs

    private:
        WebSocketServer();
};


#endif  /* WEBSOCKETSERVER_HPP_ */
