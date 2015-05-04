/*
 * WebSocketServer.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETSERVER_HPP_
#define WEBSOCKETSERVER_HPP_

#include <functional>

#include "WebSocketClient.hpp"
#include "WebSocketMessage.hpp"

typedef std::function<void(const WebSocketMessage&)> MessageHandler;

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
