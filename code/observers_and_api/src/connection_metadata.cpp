/*
 * connection_metadata.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#include "connection_metadata.hpp"

ConnectionMetadata::ConnectionMetadata(
        int id, websocketpp::connection_hdl hdl, const std::string& uri)
          : m_id(id)
          , m_hdl(hdl)
          , m_status("Connecting...")
          , m_uri(uri)
          , m_server("N/A")
          , m_error_reason("")
          ,m_messages(){}

void ConnectionMetadata::on_open(client * c, websocketpp::connection_hdl hdl)
{
    m_status = "Open";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
}

void ConnectionMetadata::on_fail(client * c, websocketpp::connection_hdl hdl)
{
    m_status = "Failed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
    m_error_reason = con->get_ec().message();
}

void ConnectionMetadata::on_close(client * c, websocketpp::connection_hdl hdl)
{
    m_status = "Closed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    std::stringstream s;
    s << "close code: " << con->get_remote_close_code() << " ("
      << websocketpp::close::status::get_string(con->get_remote_close_code())
      << "), close reason: " << con->get_remote_close_reason();
    m_error_reason = s.str();
}

void ConnectionMetadata::on_message(
        websocketpp::connection_hdl, client::message_ptr msg)
{
    if (msg->get_opcode() == websocketpp::frame::opcode::text)
    {
        m_messages.push_back("<< " + msg->get_payload());
    }
    else
    {
        m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
    }
}

websocketpp::connection_hdl ConnectionMetadata::get_hdl() const
{
    return m_hdl;
}

int ConnectionMetadata::get_id() const
{
    return m_id;
}

std::string ConnectionMetadata::get_status() const
{
    return m_status;
}

void ConnectionMetadata::record_sent_message(const std::string& message)
{
        m_messages.push_back(">> " + message);
}

std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
    out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.m_messages.begin(); it != data.m_messages.end(); ++it)
    {
        out << *it << "\n";
    }
    return out;
}
