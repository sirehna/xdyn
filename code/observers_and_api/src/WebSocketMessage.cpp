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
