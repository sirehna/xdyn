/*
 * WebSocketMessageImpl.hpp
 *
 *  Created on: May 5, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETMESSAGEIMPL_HPP_
#define WEBSOCKETMESSAGEIMPL_HPP_

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

#include <ssc/macros.hpp>
#include TR1INC(memory)

struct WebSocketMessageImpl
{
    WebSocketMessageImpl();
    ~WebSocketMessageImpl();
    WebSocketMessageImpl(const WebSocketMessageImpl& rhs);
    WebSocketMessageImpl& operator=(const WebSocketMessageImpl& rhs);


    WSServer* server;
    websocketpp::connection_hdl handle;
    message_ptr message;
};

#endif  /* WEBSOCKETMESSAGEIMPL_HPP_ */
