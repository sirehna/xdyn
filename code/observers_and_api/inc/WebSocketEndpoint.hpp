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

#include "connection_metadata.hpp"

class WebSocketEndpoint
{
    public:
        WebSocketEndpoint();
        ~WebSocketEndpoint();
        int connect(std::string const & uri);
        void close(int id, websocketpp::close::status::value code, std::string reason);
        void send(const int id, const std::string& message);

        /**
         * \brief Send a vector through a web socket
         * \param[in] id Id of the connection to use
         * \param[in] vector Vector passed by values to make a copy of the vector
         *            on purpose
         */
        template<typename T>
        void send(const int id, const std::vector<T> vector)
        {
            send_vector(id, &vector[0], sizeof(T)*vector.size());
        }

        connection_metadata::ptr getMetadata(int id) const;
        std::list<int> getIds() const;
        int getFirstId() const;

    private:
        void send_vector(const int id, void const * payload, const size_t nb_of_bytes);
        typedef std::map<int,connection_metadata::ptr> IdToConnexionMap;
        client endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> websocket_thread;
        IdToConnexionMap id_to_connection;
        int next_id;
};

#endif  /* WEBSOCKETENDPOINT_HPP_ */
