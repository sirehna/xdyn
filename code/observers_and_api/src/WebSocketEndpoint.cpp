/*
 * WebSocketEndpoint.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include "WebSocketEndpoint.hpp"
#include "WebSocketObserverException.hpp"


WebSocketEndpoint::WebSocketEndpoint () : m_endpoint(), m_thread(), m_connection_list(), m_next_id(0)
{
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);
    m_endpoint.init_asio();
    m_endpoint.start_perpetual();
    m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
}

WebSocketEndpoint::~WebSocketEndpoint()
{
    m_endpoint.stop_perpetual();
    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it)
    {
        if (it->second->get_status() != "Open")
        {
            // Only close open connections
            continue;
        }
        std::cout << "> Closing connection " << it->second->get_id() << std::endl<<std::flush;
        websocketpp::lib::error_code error_code;
        m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", error_code);
        if (error_code)
        {
            std::stringstream ss;
            ss << "> Error closing connection " << it->second->get_id() << ": "
               << error_code.message() << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException,ss.str());
        }
    }
    m_thread->join();
    std::cout << "End WebSocketEndpoint::~WebSocketEndpoint()"<< std::endl<<std::flush;
}

/**
 * \param[in] uri
 * \return id the connection generated. In case of error, the value returns is -1
 * \note
 * The \param uri can looks like
 *  - "ws://localhost:9002"
 *  - "ws://localhost:9002/ws?username=me"
 */
int WebSocketEndpoint::connect(std::string const & uri)
{
    websocketpp::lib::error_code error_code;

    client::connection_ptr con = m_endpoint.get_connection(uri, error_code);

    if (error_code)
    {
        std::stringstream ss;
        ss << "> Connect initialization error: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        return -1;
    }

    int new_id = m_next_id++;
    connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), uri);
    m_connection_list[new_id] = metadata_ptr;

    con->set_open_handler(websocketpp::lib::bind(
        &connection_metadata::on_open,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_fail_handler(websocketpp::lib::bind(
        &connection_metadata::on_fail,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_close_handler(websocketpp::lib::bind(
        &connection_metadata::on_close,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_message_handler(websocketpp::lib::bind(
        &connection_metadata::on_message,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2
    ));

    m_endpoint.connect(con);

    return new_id;
}

void WebSocketEndpoint::close(int id, websocketpp::close::status::value code, std::string reason)
{
    websocketpp::lib::error_code error_code;
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    m_endpoint.close(metadata_it->second->get_hdl(), code, reason, error_code);
    if (error_code)
    {
        std::stringstream ss;
        ss << "> Error initiating close: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
    }
}

void WebSocketEndpoint::send(const int id, const std::string& message)
{
    websocketpp::lib::error_code error_code;
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
    auto hdl = metadata_it->second->get_hdl();
    m_endpoint.send(hdl, message, websocketpp::frame::opcode::text, error_code);
    if (error_code)
    {
        std::stringstream ss;
        ss << "> Error sending message: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        return;
    }
    metadata_it->second->record_sent_message(message);
}

connection_metadata::ptr WebSocketEndpoint::getMetadata(int id) const
{
    con_list::const_iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        return connection_metadata::ptr();
    }
    else
    {
        return metadata_it->second;
    }
}

std::list<int> WebSocketEndpoint::getIds() const
{
    std::list<int> list;
    for (con_list::const_iterator metadata_it = m_connection_list.begin();metadata_it!=m_connection_list.end();++metadata_it)
    {
        list.push_back(metadata_it->first);
    }
    return list;
}

int WebSocketEndpoint::getFirstId() const
{
    con_list::const_iterator metadata_it = m_connection_list.begin();
    return metadata_it->first;
}
