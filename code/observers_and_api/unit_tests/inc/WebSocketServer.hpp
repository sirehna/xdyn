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

#include "WebSocketClient.hpp"

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

struct Message
{
    Message() : server(), handle(), message(), socket() {}
    ~Message() {}
    Message(const Message& rhs) : server(rhs.server), handle(rhs.handle), message(rhs.message), socket(rhs.socket) {}
    Message& operator=(const Message& rhs)
    {
        server = rhs.server;
        handle = rhs.handle;
        message = rhs.message;
        socket = rhs.socket;
        return *this;
    }
    WSServer* server;
    websocketpp::connection_hdl handle;
    message_ptr message;
    TR1(shared_ptr)<WebSocketClient> socket;
};

typedef std::function<void(const Message&)> MessageHandler;

class WebSocketServer
{
    public:
        WebSocketServer(const MessageHandler& message_handler, const std::string& address, const short unsigned int port);
        ~WebSocketServer();

    private:
        struct Impl;
        TR1(shared_ptr)<Impl> pimpl;
        WebSocketServer();
};


#endif  /* WEBSOCKETSERVER_HPP_ */
