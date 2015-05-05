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

namespace ssc
{
    namespace websocket
    {
        struct MessageHandler // What should we do when we receive a message?
        {
            virtual ~MessageHandler() {}
            virtual void operator()(const WebSocketMessage&) = 0;
        };

        class WebSocketServer
        {
            public:
                WebSocketServer(MessageHandler& message_handler, const std::string& address, const short unsigned int port);
                WebSocketServer(const std::string& address, const short unsigned int port);
                ~WebSocketServer();

            private:
                struct Impl;
                TR1(shared_ptr)<Impl> pimpl;
                WebSocketServer();
        };
    }
}

#endif  /* WEBSOCKETSERVER_HPP_ */
