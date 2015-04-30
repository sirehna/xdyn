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
#include "WebSocketObserverException.hpp"

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
            websocketpp::lib::error_code error_code;
            con_list::iterator metadata_it = m_connection_list.find(id);
            if (metadata_it == m_connection_list.end())
            {
                std::cout << "> No connection found with id " << id << std::endl;
                return;
            }
            auto hdl = metadata_it->second->get_hdl();
            const size_t s = vector.size();
            m_endpoint.send(hdl, &vector[0], sizeof(T)*s, websocketpp::frame::opcode::binary, error_code);
            if (error_code)
            {
                std::stringstream ss;
                ss << "> Error sending message: " << error_code.message() << std::endl;
                THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
                return;
            }
        }
        connection_metadata::ptr getMetadata(int id) const;
        std::list<int> getIds() const;
        int getFirstId() const;

    private:
        typedef std::map<int,connection_metadata::ptr> con_list;
        client m_endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
        con_list m_connection_list;
        int m_next_id;
};

#endif  /* WEBSOCKETENDPOINT_HPP_ */
