#include <websocketpp/common/thread.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <iostream>
#include <functional> // std::function
#include <unistd.h> // std::function

typedef websocketpp::server<websocketpp::config::asio> WSServer;
typedef WSServer::message_ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

/*GJ*/
#define STR(s) STR_(s)
#define STR_(s) #s
#define COUT(val) std::cout << "in file " << __FILE__ << ":" << __LINE__ << " " << #val << " = " << val << std::endl;

#define ADDRESS 127.0.0.1
#define PORT 9666

void createServerEcho(WSServer& echo_server, std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> f);
void createServerEcho(WSServer& echo_server, std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )> f)
{
    COUT("");
    echo_server.set_reuse_addr(true);
    COUT("");
    // Set logging settings
    echo_server.clear_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_error_channels(websocketpp::log::elevel::all);
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    COUT("");
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    COUT("");
    // Initialize ASIO
    echo_server.init_asio();
    // Register our message handler
    COUT("");
    echo_server.set_message_handler(bind(f,&echo_server,::_1,::_2));
    // Listen on port
    COUT("");
    COUT(STR(ADDRESS));
    COUT(STR(PORT));
    websocketpp::lib::error_code e;
    echo_server.listen(STR(ADDRESS),STR(PORT),e);
    COUT(e.message());
    // Start the server accept loop
    COUT("");
    echo_server.start_accept();
    // Start the ASIO io_service run loop
    COUT("");
    echo_server.run();
    COUT("");
}

void on_message_string(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg);
void on_message_string(WSServer* s, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
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

/****/

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
    //server.stop_listening();
    //COUT("");
    server.stop();
    COUT("");
    threadServer.join();
    COUT("");
}

int main()
{
    WebSocketServer * w= new WebSocketServer(std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )>(on_message_string));
    for (size_t i=0;i<30;++i)
    {
        usleep(100000);
    }
    delete w;
    
    /*
    WSServer * server = new WSServer;
    createServerEcho(*server, std::function<void(WSServer* , websocketpp::connection_hdl, message_ptr )>(on_message_string));
    server->stop();
    delete server;
    */
}
