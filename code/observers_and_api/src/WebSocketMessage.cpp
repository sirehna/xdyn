/*
 * WebSocketMessage.cpp
 *
 *  Created on: May 4, 2015
 *      Author: cady
 */


#include "WebSocketMessage.hpp"

WebSocketMessage::WebSocketMessage()
    :server()
    ,handle()
    ,message()
    ,socket()
{}

void WebSocketMessage::send_text(const std::string& payload) const
{
    server->send(handle, payload, websocketpp::frame::opcode::TEXT);
}

void WebSocketMessage::send_binary_string(const std::string& payload) const
{
    server->send(handle, payload, websocketpp::frame::opcode::BINARY);
}

WebSocketMessage::~WebSocketMessage()
{
}

WebSocketMessage::WebSocketMessage(const WebSocketMessage& rhs)
    :server(rhs.server)
    ,handle(rhs.handle)
    ,message(rhs.message)
    ,socket(rhs.socket)
{
}

WebSocketMessage& WebSocketMessage::operator=(const WebSocketMessage& rhs)
{
    server = rhs.server;
    handle = rhs.handle;
    message = rhs.message;
    socket = rhs.socket;
    return *this;
}
