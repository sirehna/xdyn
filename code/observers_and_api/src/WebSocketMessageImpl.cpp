/*
 * WebSocketMessageImpl.cpp
 *
 *  Created on: May 5, 2015
 *      Author: cady
 */


#include "WebSocketMessageImpl.hpp"

WebSocketMessageImpl::WebSocketMessageImpl() :
         server()
        ,handle()
        ,message()
{}

WebSocketMessageImpl::~WebSocketMessageImpl(){}

WebSocketMessageImpl::WebSocketMessageImpl(const WebSocketMessageImpl& rhs) : server(rhs.server), handle(rhs.handle), message(rhs.message)
{
}

WebSocketMessageImpl& WebSocketMessageImpl::operator=(const WebSocketMessageImpl& rhs)
{
    if (&rhs != this)
    {
        server = rhs.server;
        handle = rhs.handle;
        message = rhs.message;
    }
    return *this;
}



