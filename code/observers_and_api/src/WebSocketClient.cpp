/*
 * WebSocketEndpoint.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */

#include <unistd.h> //usleep

#include "WebSocketClient.hpp"
#include "WebSocketException.hpp"

std::string append_port_to_address(const std::string& address, const short unsigned int port);
std::string append_port_to_address(const std::string& address, const short unsigned int port)
{
    std::stringstream ss;
    ss << address << ":" << port;
    return ss.str();
}

WebSocketClient::WebSocketClient(std::string address, const short unsigned int port) : endpoint(), websocket_thread(), id_to_connection(), next_id(0)
{
    address = append_port_to_address(address, port);
    endpoint.clear_access_channels(websocketpp::log::alevel::all);
    endpoint.clear_error_channels(websocketpp::log::elevel::all);
    endpoint.init_asio();
    endpoint.start_perpetual();
    websocket_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &endpoint);
    usleep(10000);
    size_t k=0;
    connect(address);
    while(true)
    {
        ConnectionMetadata::ptr metadata = get_metadata(next_id);
        k++;
        if (k>100)
        {
            std::stringstream ss;
            ss << "Time out when retrieving metadata from the endpoint" << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
        }
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
    ConnectionMetadata::ptr metadata = get_metadata(next_id);
    if (not(metadata))
    {
        std::stringstream ss;
        ss << "Unknown connection id : " << next_id << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
    }
}

void WebSocketClient::close(const ConnectionMetadata::ptr& connexion)
{
    // Only close open connections
    if (connexion->get_status() != "Open") return;
    websocketpp::lib::error_code error_code;
    endpoint.close(connexion->get_hdl(), websocketpp::close::status::going_away, "", error_code);
    if (error_code)
    {
        std::stringstream ss;
        ss << "> Error closing connection " << connexion->get_id() << ": "
           << error_code.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketException,ss.str());
    }
}

WebSocketClient::~WebSocketClient()
{
    endpoint.stop_perpetual();
    for (const auto id2connection:id_to_connection) close(id2connection.second);
    websocket_thread->join();
}

bool WebSocketClient::good() const
{
    return next_id != -1;
}

/**
 * \param[in] uri
 * \note
 * The \param uri can looks like
 *  - "ws://localhost:9002"
 *  - "ws://localhost:9002/ws?username=me"
 */
void WebSocketClient::connect(std::string const & uri)
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
    ConnectionMetadata::ptr metadata_ptr = websocketpp::lib::make_shared<ConnectionMetadata>(next_id, con->get_handle(), uri);
    id_to_connection[next_id] = metadata_ptr;

    con->set_open_handler(websocketpp::lib::bind(
        &ConnectionMetadata::on_open,
        metadata_ptr,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_fail_handler(websocketpp::lib::bind(
        &ConnectionMetadata::on_fail,
        metadata_ptr,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_close_handler(websocketpp::lib::bind(
        &ConnectionMetadata::on_close,
        metadata_ptr,
        &endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_message_handler(websocketpp::lib::bind(
        &ConnectionMetadata::on_message,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2
    ));

    endpoint.connect(con);
}

void WebSocketClient::send(const int id, const std::string& message)
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

void WebSocketClient::send(const std::string& message)
{
    send(next_id, message);
}

ConnectionMetadata::ptr WebSocketClient::get_metadata(const int id) const
{
    IdToConnexionMap::const_iterator metadata_it = id_to_connection.find(id);
    if (metadata_it == id_to_connection.end())
    {
        return ConnectionMetadata::ptr();
    }
    else
    {
        return metadata_it->second;
    }
}

void WebSocketClient::send_vector(const int id, void const * payload, const size_t nb_of_bytes)
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
