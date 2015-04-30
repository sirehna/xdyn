/*
 * WebSocketEndpoint.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include <unistd.h> //usleep

#include "WebSocketEndpoint.hpp"
#include "WebSocketObserverException.hpp"

WebSocketEndpoint::WebSocketEndpoint(const std::string& address) : endpoint(), websocket_thread(), id_to_connection(), next_id(0)
{
    endpoint.clear_access_channels(websocketpp::log::alevel::all);
    endpoint.clear_error_channels(websocketpp::log::elevel::all);
    endpoint.init_asio();
    endpoint.start_perpetual();
    websocket_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &endpoint);
    usleep(10000);
    size_t k=0;
    std::cout << "Starting server at: " << address<<std::endl<<std::flush;
    connect(address);
    while(true)
    {
        connection_metadata::ptr metadata = get_metadata(get_current_id());
        k++;
        if (k>100)
        {
            std::stringstream ss;
            ss << "Time out when retrieving metadata from the endpoint" << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
        }
        std::cout<<metadata->get_status()<<std::endl;
        if (metadata->get_status()=="Open")
        {
            break;
        }
        else if (metadata->get_status()=="Failed")
        {
            std::stringstream ss;
            ss << "Connection failed" <<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
            break;
        }
        usleep(100000);
    }
    connection_metadata::ptr metadata = get_metadata(get_current_id());
    if (metadata)
    {
        std::cout << *metadata << std::endl<<std::flush;
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown connection id : " << get_current_id()<<std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }
}

WebSocketEndpoint::~WebSocketEndpoint()
{
    endpoint.stop_perpetual();
    for (IdToConnexionMap::const_iterator it = id_to_connection.begin(); it != id_to_connection.end(); ++it)
    {
        if (it->second->get_status() != "Open")
        {
            // Only close open connections
            continue;
        }
        std::cout << "> Closing connection " << it->second->get_id() << std::endl<<std::flush;
        websocketpp::lib::error_code error_code;
        endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", error_code);
        if (error_code)
        {
            std::stringstream ss;
            ss << "> Error closing connection " << it->second->get_id() << ": "
               << error_code.message() << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException,ss.str());
        }
    }
    websocket_thread->join();
    std::cout << "End WebSocketEndpoint::~WebSocketEndpoint()"<< std::endl<<std::flush;
}

int WebSocketEndpoint::get_current_id() const
{
    return next_id;
}

bool WebSocketEndpoint::good() const
{
    return next_id != -1;
}

/**
 * \param[in] uri
 * \return id the connection generated. In case of error, the value returns is -1
 * \note
 * The \param uri can looks like
 *  - "ws://localhost:9002"
 *  - "ws://localhost:9002/ws?username=me"
 */
void WebSocketEndpoint::connect(std::string const & uri)
{
    websocketpp::lib::error_code error_code;

    client::connection_ptr con = endpoint.get_connection(uri, error_code);

    if (error_code)
    {
        std::stringstream ss;
        ss << "> Connect initialization error: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }

    next_id++;
    connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(next_id, con->get_handle(), uri);
    id_to_connection[next_id] = metadata_ptr;

    con->set_open_handler(websocketpp::lib::bind(
        &connection_metadata::on_open,
        metadata_ptr,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_fail_handler(websocketpp::lib::bind(
        &connection_metadata::on_fail,
        metadata_ptr,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_close_handler(websocketpp::lib::bind(
        &connection_metadata::on_close,
        metadata_ptr,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_message_handler(websocketpp::lib::bind(
        &connection_metadata::on_message,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2
    ));

    endpoint.connect(con);
}

void WebSocketEndpoint::close(const int id, websocketpp::close::status::value code, std::string reason)
{
    websocketpp::lib::error_code error_code;
    IdToConnexionMap::iterator metadata_it = id_to_connection.find(id);
    if (metadata_it == id_to_connection.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    endpoint.close(metadata_it->second->get_hdl(), code, reason, error_code);
    if (error_code)
    {
        std::stringstream ss;
        ss << "> Error initiating close: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }
}

void WebSocketEndpoint::send(const int id, const std::string& message)
{
    websocketpp::lib::error_code error_code;
    IdToConnexionMap::iterator metadata_it = id_to_connection.find(id);
    if (metadata_it == id_to_connection.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
    auto hdl = metadata_it->second->get_hdl();
    endpoint.send(hdl, message, websocketpp::frame::opcode::text, error_code);
    if (error_code)
    {
        std::stringstream ss;
        ss << "> Error sending message: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
        return;
    }
    metadata_it->second->record_sent_message(message);
}

void WebSocketEndpoint::send(const std::string& message)
{
    send(get_current_id(), message);
}

connection_metadata::ptr WebSocketEndpoint::get_metadata(const int id) const
{
    IdToConnexionMap::const_iterator metadata_it = id_to_connection.find(id);
    if (metadata_it == id_to_connection.end())
    {
        return connection_metadata::ptr();
    }
    else
    {
        return metadata_it->second;
    }
}

std::list<int> WebSocketEndpoint::get_ids() const
{
    std::list<int> list;
    for (IdToConnexionMap::const_iterator metadata_it = id_to_connection.begin();metadata_it!=id_to_connection.end();++metadata_it)
    {
        list.push_back(metadata_it->first);
    }
    return list;
}

int WebSocketEndpoint::get_first_id() const
{
    IdToConnexionMap::const_iterator metadata_it = id_to_connection.begin();
    return metadata_it->first;
}

void WebSocketEndpoint::send_vector(const int id, void const * payload, const size_t nb_of_bytes)
{
    websocketpp::lib::error_code error_code;
    IdToConnexionMap::iterator metadata_it = id_to_connection.find(id);
    if (metadata_it == id_to_connection.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
    auto handle = metadata_it->second->get_hdl();
    endpoint.send(handle, payload, nb_of_bytes, websocketpp::frame::opcode::binary, error_code);
    if (error_code)
    {
        std::stringstream ss;
        ss << "> Error sending message: " << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
        return;
    }
}
