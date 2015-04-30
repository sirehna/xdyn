#include "WebSocketObserver.hpp"
#include "WebSocketObserverTest.hpp"
#include "WebSocketObserverException.hpp"

#include <iostream>
#include <unistd.h> //usleep
#include <functional> // std::function

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::thread;

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

#ifndef STR
#define STR(s) STR_(s)
#endif

#ifndef STR_
#define STR_(s) #s
#endif

#define ADDRESS 127.0.0.1
#define PORT 9002
#define MESSAGE_SENT "First message"

void createServerEcho(WSServer& echo_server, std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> f);
void createServerEcho(WSServer& echo_server, std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> f)
{
    echo_server.set_reuse_addr(true);
    // Set logging settings
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // Initialize ASIO
    echo_server.init_asio();
    // Register our message handler
    echo_server.set_message_handler(bind(f,&echo_server,::_1,::_2));
    // Listen on port
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(STR(ADDRESS)), PORT);
    echo_server.listen(ep);
    // Start the server accept loop
    echo_server.start_accept();
    // Start the ASIO io_service run loop
    echo_server.run();
}

int connectToServer(WebSocketEndpoint& endpoint);
int connectToServer(WebSocketEndpoint& endpoint)
{
    usleep(10000);
    size_t k=0;
    const std::string connectAddress(std::string("ws://")+std::string(STR(ADDRESS))+std::string(":")+std::string(STR(PORT)));
    std::cout << "Start creating observer :" << connectAddress<<std::endl<<std::flush;
    int id = endpoint.connect(connectAddress);
    while(true)
    {
        connection_metadata::ptr metadata = endpoint.getMetadata(id);
        k++;
        if (k>100)
        {
            std::stringstream ss;
            ss << "Time out" <<id<<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
        }
        std::cout<<metadata->get_status()<<std::endl;
        if (metadata->get_status()=="Open")
        {
            break;
        }
        else if (metadata->get_status()=="Failed")
        {
            std::stringstream ss;
            ss << "Connection failed" <<id<<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
            break;
        }
        usleep(100000);
    }
    COUT(k);
    connection_metadata::ptr metadata = endpoint.getMetadata(id);
    if (metadata)
    {
        std::cout << *metadata << std::endl<<std::flush;
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown connection id : " <<id<<std::endl;
        THROW(__PRETTY_FUNCTION__, WebSocketObserverException, ss.str());
    }
    return id;
}

class WebSocketServer
{
    public:
        WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> m);
        ~WebSocketServer();
    protected:
        WSServer server;
        websocketpp::lib::thread threadServer;
    private:
        WebSocketServer();
};

WebSocketServer::WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> m):
        server(),
        threadServer(createServerEcho,std::ref(server),m)
{
}

WebSocketServer::~WebSocketServer()
{
    std::cout<<"Calling WebSocketServer::~WebSocketServer"<<std::endl<<std::flush;
    server.stop();
    threadServer.join();
}

void on_message_string(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg);
void on_message_string(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
    ASSERT_EQ(MESSAGE_SENT, msg->get_payload());
    try
    {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    }
    catch (const websocketpp::lib::error_code& e)
    {
        std::cout << "Echo failed because: " << e
                  << "(" << e.message() << ")" << std::endl;
    }
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_connect_a_web_socket_server)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )>(on_message_string)));
    {
        WebSocketEndpoint endpoint;
        const int id = connectToServer(endpoint);
        ASSERT_NE(-1, id);
    }
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_send_a_string)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )>(on_message_string)));
    {
        WebSocketEndpoint endpoint;
        const int id = connectToServer(endpoint);
        endpoint.send(id, MESSAGE_SENT);
    }
}

template<typename T>
std::string convertStdVectorToStdString(const std::vector<T>& v);
template<typename T>
typename std::vector<T> convertStdStringToStdVector(const std::string& s);

template<typename T>
std::string convertStdVectorToStdString(const std::vector<T>& v)
{
    const size_t ss = v.size()*sizeof(T);
    std::string s(ss, 0);
    memcpy((void*)s.c_str(),v.data(),ss);
    return s;
}

template<typename T>
typename std::vector<T> convertStdStringToStdVector(const std::string& s)
{
    std::vector<T> res;
    const size_t n = s.size()/sizeof(T);
    res.resize(n);
    for (size_t i=0;i<n;++i)
    {
        res[i]=*reinterpret_cast<const double*>(s.c_str() + sizeof(T)*i);
    }
    return res;
}

void on_message_vector(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg);
void on_message_vector(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    ASSERT_EQ(websocketpp::frame::opcode::binary, msg->get_opcode());
    const std::string payload = msg->get_payload();
    std::vector<double> vv = convertStdStringToStdVector<double>(payload);
    ASSERT_EQ(3,payload.size()/8);
    ASSERT_EQ(3,vv.size());
    ASSERT_EQ(1.0,vv[0]);
    ASSERT_EQ(2.0,vv[1]);
    ASSERT_EQ(3.0,vv[2]);
    try
    {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    }
    catch (const websocketpp::lib::error_code& e)
    {
        std::cout << "Echo failed because: " << e
                  << "(" << e.message() << ")" << std::endl;
    }
}

TEST_F(WebSocketObserverTest, WebSocketEndpoint_should_be_able_to_send_a_vector_doubles)
{
    TR1(shared_ptr)<WebSocketServer> w(new WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )>(on_message_vector)));
    {
        std::vector<double> v(3,0.0);
        v[0]=1.0;v[1]=2.0;v[2]=3.0;
        WebSocketEndpoint endpoint;
        const int id = connectToServer(endpoint);
        endpoint.send(id, v);
    }
}

TEST_F(WebSocketObserverTest, should_be_able_to_convert_vector_of_doubles_to_string_and_back)
{
    const size_t n=3;
    std::vector<double> v(n,0.0);
    v[0]=1.0;v[1]=2.0;v[2]=3.0;
    const std::string s = convertStdVectorToStdString(v);
    ASSERT_EQ(3*8,s.size());
    std::vector<double> v2=convertStdStringToStdVector<double>(s);
    ASSERT_EQ(n,v2.size());
    for (size_t i=0;i<n;++i)
    {
        ASSERT_EQ(v[i],v2[i]);
    }
}
