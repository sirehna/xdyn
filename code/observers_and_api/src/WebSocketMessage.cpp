/*
 * WebSocketMessage.cpp
 *
 *  Created on: May 4, 2015
 *      Author: cady
 */

#include "WebSocketMessage.hpp"
#include "WebSocketMessageImpl.hpp"

using namespace ssc::websocket;

Message::Message()
    :pimpl(new WebSocketMessageImpl())
{}

Message::Message(const WebSocketMessageImpl& pimpl_)
    :pimpl(new WebSocketMessageImpl(pimpl_))
{}

void Message::send_text(const std::string& payload) const
{
    pimpl->server->send(pimpl->handle, payload, websocketpp::frame::opcode::TEXT);
}

std::string Message::get_payload() const
{
    return pimpl->message->get_payload();
}

void Message::send_binary_string(const std::string& payload) const
{
    pimpl->server->send(pimpl->handle, payload, websocketpp::frame::opcode::BINARY);
}
