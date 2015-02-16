#ifndef WEBSOCKETOBSERVER_HPP_
#define WEBSOCKETOBSERVER_HPP_

#include <list>
#include <ssc/exception_handling.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include "Observer.hpp"

class WebSocketObserverException: public ::ssc::exception_handling::Exception
{
    public:
        WebSocketObserverException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

typedef websocketpp::client<websocketpp::config::asio_client> client;

class connection_metadata
{
    public:
        typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;
        connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri);
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

class WebSocketEndpoint
{
    public:
        WebSocketEndpoint();
        ~WebSocketEndpoint();
        int connect(std::string const & uri);
        void close(int id, websocketpp::close::status::value code, std::string reason);
        void send(const int id, const std::string& message);
        void send(const int id, const std::vector<double>& vector);
        connection_metadata::ptr get_metadata(int id) const;
        std::list<int> get_ids() const;
    private:
        typedef std::map<int,connection_metadata::ptr> con_list;
        client m_endpoint;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
        con_list m_connection_list;
        int m_next_id;
};

class WebSocketObserver : public Observer
{
    public:
        WebSocketObserver(const std::string& address, const std::vector<std::string>& data);
        ~WebSocketObserver();
        void send(const std::string& message);
        void send(const std::vector<double>& vector);
        friend std::ostream & operator<< (std::ostream & out, WebSocketObserver const & wsObserver);
    private:
        WebSocketEndpoint endpoint;
        int id;
        void flush_after_initialization();
        void flush_after_write();
        void flush_value();
        std::function<void()> get_serializer(const double val, const DataAddressing&);
        std::function<void()> get_initializer(const double, const DataAddressing& address);
        std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing&);
        std::function<void()> get_initializer(const SurfaceElevationGrid&, const DataAddressing& address);
};

#endif /* WEBSOCKETOBSERVER_HPP_ */
