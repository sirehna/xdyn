/*
 * WebSocketMessage.hpp
 *
 *  Created on: May 4, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETMESSAGE_HPP_
#define WEBSOCKETMESSAGE_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "WebSocketClient.hpp"
#include "string2vector.hpp"

struct WebSocketMessageImpl;

namespace ssc
{
    namespace websocket
    {
        struct Message
        {
            Message();
            Message(const WebSocketMessageImpl& pimpl);
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

            private:
                TR1(shared_ptr)<WebSocketMessageImpl> pimpl;
                void send_binary_string(const std::string& s) const;
        };
    }
}

#endif  /* WEBSOCKETMESSAGE_HPP_ */
