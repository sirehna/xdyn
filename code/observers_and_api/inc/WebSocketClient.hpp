/*
 * WebSocketEndpoint.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETENDPOINT_HPP_
#define WEBSOCKETENDPOINT_HPP_

#include <string>
#include <vector>

#include <ssc/macros.hpp>
#include TR1INC(memory)

class WebSocketClient
{
    public:
        WebSocketClient(const std::string& address, const short unsigned int port);
        ~WebSocketClient();
        void send_text(const std::string& message); // Sends to current socket
        bool good() const; // Returns true if the connection is successfully established

        /**
         * \brief Send a vector through a web socket
         * \param[in] id Id of the connection to use
         * \param[in] vector Vector passed by values to make a copy of the vector
         *            on purpose
         */
        template<typename T>
        void send_binary(const std::vector<T> vector)
        {
            send_vector(&vector[0], sizeof(T)*vector.size());
        }

    private:
        WebSocketClient();
        struct Impl;
        TR1(shared_ptr)<Impl> pimpl;
        void send_vector(const int id, void const * payload, const size_t nb_of_bytes);
        void send_vector(void const * payload, const size_t nb_of_bytes);
        template<typename T>
        void send(const int id, const std::vector<T> vector)
        {
            send_vector(id, &vector[0], sizeof(T)*vector.size());
        }
};

#endif  /* WEBSOCKETENDPOINT_HPP_ */
