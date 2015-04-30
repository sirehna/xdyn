/*
 * connection_metadata.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#include "ConnectionMetadata.hpp"

ConnectionMetadata::ConnectionMetadata(
        int id, websocketpp::connection_hdl hdl, const std::string& uri)
          : m_id(id)
          , hdl(hdl)
          , status("Connecting...")
          , uri(uri)
          , server("N/A")
          , error_reason("")
          ,messages(){}

void ConnectionMetadata::on_open(client * c, websocketpp::connection_hdl hdl)
{
    status = "Open";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    server = con->get_response_header("Server");
}

void ConnectionMetadata::on_fail(client * c, websocketpp::connection_hdl hdl)
{
    status = "Failed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    server = con->get_response_header("Server");
    error_reason = con->get_ec().message();
}

void ConnectionMetadata::on_close(client * c, websocketpp::connection_hdl hdl)
{
    status = "Closed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    std::stringstream s;
    s << "close code: " << con->get_remote_close_code() << " ("
      << websocketpp::close::status::get_string(con->get_remote_close_code())
      << "), close reason: " << con->get_remote_close_reason();
    error_reason = s.str();
}

void ConnectionMetadata::on_message(
        websocketpp::connection_hdl, client::message_ptr msg)
{
    if (msg->get_opcode() == websocketpp::frame::opcode::text)
    {
        messages.push_back("<< " + msg->get_payload());
    }
    else
    {
        messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
    }
}

websocketpp::connection_hdl ConnectionMetadata::get_hdl() const
{
    return hdl;
}

int ConnectionMetadata::get_id() const
{
    return m_id;
}

std::string ConnectionMetadata::get_status() const
{
    return status;
}

void ConnectionMetadata::record_sent_message(const std::string& message)
{
        messages.push_back(">> " + message);
}

std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data) {
    out << "> URI: " << data.uri << "\n"
        << "> Status: " << data.status << "\n"
        << "> Remote Server: " << (data.server.empty() ? "None Specified" : data.server) << "\n"
        << "> Error/close reason: " << (data.error_reason.empty() ? "N/A" : data.error_reason) << "\n";
    out << "> Messages Processed: (" << data.messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.messages.begin(); it != data.messages.end(); ++it)
    {
        out << *it << "\n";
    }
    return out;
}
