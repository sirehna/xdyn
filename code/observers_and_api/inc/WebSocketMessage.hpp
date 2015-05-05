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
#include "string2vector.hpp"

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

struct WebSocketMessage
{
    WebSocketMessage();

    void send_text(const std::string& payload) const;
    template<typename T>
    void send_binary(const std::vector<T> vector) const
    {
        const std::string s = convert_vector_to_string(vector);
        send_binary_string(s);
    }

    std::string get_payload() const;

    template <typename T> std::vector<T> get_binary_payload() const
    {
        const std::string payload = get_payload();
        return convert_string_to_vector<T>(payload);
    }


    ~WebSocketMessage();
    WebSocketMessage(const WebSocketMessage& rhs);
    WebSocketMessage& operator=(const WebSocketMessage& rhs);


    void send_binary_string(const std::string& s) const;

    WSServer* server;
    websocketpp::connection_hdl handle;
    message_ptr message;
    TR1(shared_ptr)<WebSocketClient> socket;
};

#endif  /* WEBSOCKETMESSAGE_HPP_ */
