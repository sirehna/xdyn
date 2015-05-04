/*
 * WebSocketMessage.hpp
 *
 *  Created on: May 4, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETMESSAGE_HPP_
#define WEBSOCKETMESSAGE_HPP_

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "WebSocketClient.hpp"

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

struct Message
{
    Message();
    ~Message();
    Message(const Message& rhs);
    Message& operator=(const Message& rhs);

    WSServer* server;
    websocketpp::connection_hdl handle;
    message_ptr message;
    TR1(shared_ptr)<WebSocketClient> socket;
};

#endif  /* WEBSOCKETMESSAGE_HPP_ */
