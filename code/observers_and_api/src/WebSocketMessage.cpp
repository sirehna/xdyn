/*
 * WebSocketMessage.cpp
 *
 *  Created on: May 4, 2015
 *      Author: cady
 */

#include "WebSocketInternalTypes.hpp"
#include "WebSocketMessage.hpp"
#include "WebSocketMessageImpl.hpp"

WebSocketMessage::WebSocketMessage()
    :pimpl(new WebSocketMessageImpl())
{}

WebSocketMessage::WebSocketMessage(const WebSocketMessageImpl& pimpl_)
    :pimpl(new WebSocketMessageImpl(pimpl_))
{}

void WebSocketMessage::send_text(const std::string& payload) const
{
    pimpl->server->send(pimpl->handle, payload, websocketpp::frame::opcode::TEXT);
}

std::string WebSocketMessage::get_payload() const
{
    return pimpl->message->get_payload();
}

void WebSocketMessage::send_binary_string(const std::string& payload) const
{
    pimpl->server->send(pimpl->handle, payload, websocketpp::frame::opcode::BINARY);
}
