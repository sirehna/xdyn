/*
 * connection_metadata.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef CONNECTION_METADATA_HPP_
#define CONNECTION_METADATA_HPP_

#include <list>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;

class connection_metadata
{
    public:
        typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;
        connection_metadata(int id, websocketpp::connection_hdl hdl, const std::string& uri);
        void on_open(client * c, websocketpp::connection_hdl hdl);
        void on_fail(client * c, websocketpp::connection_hdl hdl);
        void on_close(client * c, websocketpp::connection_hdl hdl);
        void on_message(websocketpp::connection_hdl, client::message_ptr msg);
        websocketpp::connection_hdl get_hdl() const;
        int get_id() const;
        std::string get_status() const;
        void record_sent_message(const std::string& message);

        friend std::ostream & operator<< (std::ostream & out, connection_metadata const & data);
    private:
        int m_id;
        websocketpp::connection_hdl m_hdl;
        std::string m_status;
        std::string m_uri;
        std::string m_server;
        std::string m_error_reason;
        std::vector<std::string> m_messages;
};

#endif  /* CONNECTION_METADATA_HPP_ */
