#include <websocketpp/common/thread.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <iostream>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (const websocketpp::lib::error_code& e) {
        std::cout << "Echo failed because: " << e
                  << "(" << e.message() << ")" << std::endl;
    }
}


typedef websocketpp::client<websocketpp::config::asio_client> client;

class ConnectionMetadata {
public:
    typedef websocketpp::lib::shared_ptr<ConnectionMetadata> ptr;

    ConnectionMetadata(int id, websocketpp::connection_hdl hdl, std::string uri)
        : m_id(id)
        , hdl(hdl)
        , status("Connecting")
        , uri(uri)
        , server("N/A")
        , error_reason("")
        ,messages()
    {}

    void on_open(client * c, websocketpp::connection_hdl hdl) {
        status = "Open";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        server = con->get_response_header("Server");
    }

    void on_fail(client * c, websocketpp::connection_hdl hdl) {
        status = "Failed";

        client::connection_ptr con = c->get_con_from_hdl(hdl);
        server = con->get_response_header("Server");
        error_reason = con->get_ec().message();
    }

    void on_close(client * c, websocketpp::connection_hdl hdl) {
        status = "Closed";
        client::connection_ptr con = c->get_con_from_hdl(hdl);
        std::stringstream s;
        s << "close code: " << con->get_remote_close_code() << " ("
          << websocketpp::close::status::get_string(con->get_remote_close_code())
          << "), close reason: " << con->get_remote_close_reason();
        error_reason = s.str();
    }

    void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
        if (msg->get_opcode() == websocketpp::frame::opcode::text) {
            messages.push_back("<< " + msg->get_payload());
        } else {
            messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
        }
    }

    websocketpp::connection_hdl get_hdl() const {
        return hdl;
    }

    int get_id() const {
        return m_id;
    }

    std::string get_status() const {
        return status;
    }

    void record_sent_message(std::string message) {
        messages.push_back(">> " + message);
    }

    friend std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data);
private:
    int m_id;
    websocketpp::connection_hdl hdl;
    std::string status;
    std::string uri;
    std::string server;
    std::string error_reason;
    std::vector<std::string> messages;
};

std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data) {
    out << "> URI: " << data.uri << "\n"
        << "> Status: " << data.status << "\n"
        << "> Remote Server: " << (data.server.empty() ? "None Specified" : data.server) << "\n"
        << "> Error/close reason: " << (data.error_reason.empty() ? "N/A" : data.error_reason) << "\n";
    out << "> Messages Processed: (" << data.messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.messages.begin(); it != data.messages.end(); ++it) {
        out << *it << "\n";
    }

    return out;
}

class websocket_endpoint {
public:
    websocket_endpoint () :  m_endpoint(), m_thread(), m_connection_list(), m_next_id(0)
    {
        m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
        m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

        m_endpoint.init_asio();
        m_endpoint.start_perpetual();

        m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
    }

    ~websocket_endpoint()
    {
        m_endpoint.stop_perpetual();
        for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
            if (it->second->get_status() != "Open") {
                // Only close open connections
                continue;
            }

            std::cout << "> Closing connection " << it->second->get_id() << std::endl;

            websocketpp::lib::error_code ec;
            m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
            if (ec) {
                std::cout << "> Error closing connection " << it->second->get_id() << ": "
                          << ec.message() << std::endl;
            }
        }
        m_thread->join();
    }

    int connect(std::string const & uri) {
        websocketpp::lib::error_code ec;

        client::connection_ptr con = m_endpoint.get_connection(uri, ec);

        if (ec) {
            std::cout << "> Connect initialization error: " << ec.message() << std::endl;
            return -1;
        }

        int new_id = m_next_id++;
        ConnectionMetadata::ptr metadata_ptr = websocketpp::lib::make_shared<ConnectionMetadata>(new_id, con->get_handle(), uri);
        m_connection_list[new_id] = metadata_ptr;

        con->set_open_handler(websocketpp::lib::bind(
            &ConnectionMetadata::on_open,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_fail_handler(websocketpp::lib::bind(
            &ConnectionMetadata::on_fail,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_close_handler(websocketpp::lib::bind(
            &ConnectionMetadata::on_close,
            metadata_ptr,
            &m_endpoint,
            websocketpp::lib::placeholders::_1
        ));
        con->set_message_handler(websocketpp::lib::bind(
            &ConnectionMetadata::on_message,
            metadata_ptr,
            websocketpp::lib::placeholders::_1,
            websocketpp::lib::placeholders::_2
        ));

        m_endpoint.connect(con);

        return new_id;
    }

    void close(int id, websocketpp::close::status::value code, std::string reason) {
        websocketpp::lib::error_code ec;

        con_list::iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            std::cout << "> No connection found with id " << id << std::endl;
            return;
        }

        m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
        if (ec) {
            std::cout << "> Error initiating close: " << ec.message() << std::endl;
        }
    }

    void send(int id, std::string message) {
        websocketpp::lib::error_code ec;

        con_list::iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            std::cout << "> No connection found with id " << id << std::endl;
            return;
        }

        m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cout << "> Error sending message: " << ec.message() << std::endl;
            return;
        }

        metadata_it->second->record_sent_message(message);
    }

    ConnectionMetadata::ptr get_metadata(int id) const {
        con_list::const_iterator metadata_it = m_connection_list.find(id);
        if (metadata_it == m_connection_list.end()) {
            return ConnectionMetadata::ptr();
        } else {
            return metadata_it->second;
        }
    }
private:
    typedef std::map<int,ConnectionMetadata::ptr> con_list;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id;
};

void createServerEcho(server& echo_server);
void createServerEcho(server& echo_server)
{
    echo_server.set_reuse_addr(true);
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    echo_server.init_asio();
    // Register our message handler
    echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));
    // Listen on port 9002
    echo_server.listen("127.0.0.1","9002");
    // Start the server accept loop
    echo_server.start_accept();
    // Start the ASIO io_service run loop
    echo_server.run();
}

int main() {
    bool done = false;
    std::string input;
    websocket_endpoint endpoint;
    server s;
    websocketpp::lib::thread threadServer (createServerEcho,std::ref(s));

    while (!done) {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            done = true;
        } else if (input == "help") {
            std::cout
                << "\nCommand List:\n"
                << "connect <ws uri>\n"
                << "send <connection id> <message>\n"
                << "close <connection id> [<close code:default=1000>] [<close reason>]\n"
                << "show <connection id>\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        } else if (input.substr(0,7) == "connect") {
            int id = endpoint.connect(input.substr(8));
            if (id != -1) {
                std::cout << "> Created connection with id " << id << std::endl;
            }
        } else if (input.substr(0,4) == "send") {
            std::stringstream ss(input);

            std::string cmd;
            int id;
            std::string message = "";

            ss >> cmd >> id;
            std::getline(ss,message);

            endpoint.send(id, message);
        } else if (input.substr(0,5) == "close") {
            std::stringstream ss(input);

            std::string cmd;
            int id;
            int close_code = websocketpp::close::status::normal;
            std::string reason = "";

            ss >> cmd >> id >> close_code;
            std::getline(ss,reason);

            endpoint.close(id, (websocketpp::close::status::value)close_code, reason);
        } else if (input.substr(0,4) == "show") {
            int id = atoi(input.substr(5).c_str());

            ConnectionMetadata::ptr metadata = endpoint.get_metadata(id);
            if (metadata) {
                std::cout << *metadata << std::endl;
            } else {
                std::cout << "> Unknown connection id " << id << std::endl;
            }
        } else {
            std::cout << "> Unrecognized Command" << std::endl;
        }
    }
    s.stop();
    threadServer.join();
    return 0;
}
