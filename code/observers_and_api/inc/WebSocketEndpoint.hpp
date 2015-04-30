/*
 * WebSocketEndpoint.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef WEBSOCKETENDPOINT_HPP_
#define WEBSOCKETENDPOINT_HPP_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include "ConnectionMetadata.hpp"

class WebSocketEndpoint
{
    public:
        WebSocketEndpoint(std::string address, const short unsigned int port);
        ~WebSocketEndpoint();
        void connect(std::string const & uri);
        void close(const int id, websocketpp::close::status::value code, std::string reason);
        void send(const std::string& message); // Sends to current socket
        bool good() const; // Returns true if the connection is successfully established

        /**
         * \brief Send a vector through a web socket
         * \param[in] id Id of the connection to use
         * \param[in] vector Vector passed by values to make a copy of the vector
         *            on purpose
         */
        template<typename T>
        void send(const std::vector<T> vector)
        {
            send_vector(next_id, &vector[0], sizeof(T)*vector.size());
        }

        ConnectionMetadata::ptr get_metadata(const int id) const;

    private:
        WebSocketEndpoint();
        void send_vector(const int id, void const * payload, const size_t nb_of_bytes);
        void close(const ConnectionMetadata::ptr& connexion);
        void send(const int id, const std::string& message);
        template<typename T>
        void send(const int id, const std::vector<T> vector)
        {
            send_vector(id, &vector[0], sizeof(T)*vector.size());
        }

        typedef std::map<int,ConnectionMetadata::ptr> IdToConnexionMap;
        client endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> websocket_thread;
        IdToConnexionMap id_to_connection;
        int next_id;
};

#endif  /* WEBSOCKETENDPOINT_HPP_ */
