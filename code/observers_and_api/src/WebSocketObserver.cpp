#include "WebSocketObserver.hpp"

connection_metadata::connection_metadata(
        int id, websocketpp::connection_hdl hdl, std::string uri)
          : m_id(id)
          , m_hdl(hdl)
          , m_status("Connecting")
          , m_uri(uri)
          , m_server("N/A")
          , m_error_reason("")
          ,m_messages(){}

void connection_metadata::on_open(client * c, websocketpp::connection_hdl hdl)
{
    m_status = "Open";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
}

void connection_metadata::on_fail(client * c, websocketpp::connection_hdl hdl)
{
    m_status = "Failed";

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
    m_error_reason = con->get_ec().message();
}

void connection_metadata::on_close(client * c, websocketpp::connection_hdl hdl)
{
    m_status = "Closed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    std::stringstream s;
    s << "close code: " << con->get_remote_close_code() << " ("
      << websocketpp::close::status::get_string(con->get_remote_close_code())
      << "), close reason: " << con->get_remote_close_reason();
    m_error_reason = s.str();
}

void connection_metadata::on_message(
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

websocketpp::connection_hdl connection_metadata::get_hdl() const
{
    return m_hdl;
}

int connection_metadata::get_id() const
{
    return m_id;
}

std::string connection_metadata::get_status() const
{
    return m_status;
}

void connection_metadata::record_sent_message(const std::string& message)
{
        m_messages.push_back(">> " + message);
}

std::ostream & operator<< (std::ostream & out, connection_metadata const & data) {
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

WebSocketEndpoint::WebSocketEndpoint () : m_endpoint(), m_thread(), m_connection_list(), m_next_id(0)
{
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);
    m_endpoint.init_asio();
    //m_endpoint.start_perpetual();
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

        std::cout << "> Closing connection " << it->second->get_id() << std::endl;

        websocketpp::lib::error_code ec;
        m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
        if (ec)
        {
            std::stringstream ss;
            ss << "> Error closing connection " << it->second->get_id() << ": "
               << ec.message() << std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException,ss.str());
        }
    }
    m_thread->join();
}

int WebSocketEndpoint::connect(std::string const & uri)
{
    websocketpp::lib::error_code ec;

    client::connection_ptr con = m_endpoint.get_connection(uri, ec);

    if (ec)
    {
        std::stringstream ss;
        ss << "> Connect initialization error: " << ec.message() << std::endl;
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
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
    if (ec)
    {
        std::stringstream ss;
        ss << "> Error initiating close: " << ec.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
    }
}

void WebSocketEndpoint::send(const int id, const std::string& message)
{
    websocketpp::lib::error_code ec;
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
    if (ec)
    {
        std::stringstream ss;
        ss << "> Error sending message: " << ec.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        return;
    }

    metadata_it->second->record_sent_message(message);
}

void WebSocketEndpoint::send(const int id, const std::vector<double>& vector)
{
    websocketpp::lib::error_code ec;
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    m_endpoint.send(metadata_it->second->get_hdl(), vector.data(),vector.size()*sizeof(double), websocketpp::frame::opcode::binary, ec);
    if (ec)
    {
        std::stringstream ss;
        ss << "> Error sending message: " << ec.message() << std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        return;
    }
    // metadata_it->second->record_sent_message(message);
}

connection_metadata::ptr WebSocketEndpoint::get_metadata(int id) const
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

std::list<int> WebSocketEndpoint::get_ids() const
{
    std::list<int> list;
    for (con_list::const_iterator metadata_it = m_connection_list.begin();metadata_it!=m_connection_list.end();++metadata_it)
    {
        list.push_back(metadata_it->first);
    }
    return list;
}

WebSocketObserver::WebSocketObserver(const std::string& address, const std::vector<std::string>& data):Observer(data),endpoint(),id(endpoint.connect(address))
{
    std::cout<<"id "<<id<<std::endl<<std::flush;
    if (id == -1)
    {
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, "WebSocketObserver failed to connect to address" + address);
    }
}

void WebSocketObserver::send(const std::string& message)
{
    this->endpoint.send(this->id,message);
}

void WebSocketObserver::send(const std::vector<double>& vector)
{
    this->endpoint.send(this->id,vector);
}

WebSocketObserver::~WebSocketObserver()
{
    std::cout<<"Destructor"<<std::endl<<std::flush;
    /*
    for (const auto id:endpoint.get_ids())
    {
        endpoint.close(id, (websocketpp::close::status::value)websocketpp::close::status::normal, "");
    }
    */
}

std::function<void()> WebSocketObserver::get_serializer(const double val, const DataAddressing&)
{
    return [this,val](){};
}

std::function<void()> WebSocketObserver::get_initializer(const double, const DataAddressing& address)
{
    return [this,address](){};
}

std::function<void()> WebSocketObserver::get_serializer(const SurfaceElevationGrid& val, const DataAddressing&)
{
    return [this,val](){};
}

std::function<void()> WebSocketObserver::get_initializer(const SurfaceElevationGrid&, const DataAddressing&address)
{
    return [this,address](){};
}

void WebSocketObserver::flush_after_initialization()
{
}

void WebSocketObserver::flush_after_write()
{
}

void WebSocketObserver::flush_value()
{
}

std::ostream & operator<< (std::ostream & out, WebSocketObserver const & wsObserver)
{
    for (const auto id:wsObserver.endpoint.get_ids())
    {
        connection_metadata::ptr metadata = wsObserver.endpoint.get_metadata(id);
        if (metadata)
        {
            out << *metadata << std::endl;
        }
        else
        {
            std::stringstream ss;
            ss << "Unknown connection id : " <<id<<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        }
    }
    return out;
}

