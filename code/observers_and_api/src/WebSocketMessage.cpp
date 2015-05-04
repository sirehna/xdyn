/*
 * WebSocketMessage.cpp
 *
 *  Created on: May 4, 2015
 *      Author: cady
 */


#include "WebSocketMessage.hpp"

Message::Message()
    :server()
    ,handle()
    ,message()
    ,socket()
{}

Message::~Message()
{
}

Message::Message(const Message& rhs)
    :server(rhs.server)
    ,handle(rhs.handle)
    ,message(rhs.message)
    ,socket(rhs.socket)
{
}

Message& Message::operator=(const Message& rhs)
{
    server = rhs.server;
    handle = rhs.handle;
    message = rhs.message;
    socket = rhs.socket;
    return *this;
}
