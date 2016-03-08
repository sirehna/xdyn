#include "WebSocketObserver.hpp"
#include <ssc/websocket.hpp>

WebSocketObserver::WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data):
DictObserver(data),socket(new ssc::websocket::Client(address, port))
{
    if (not(socket->good()))
    {
        THROW(__PRETTY_FUNCTION__, ssc::websocket::WebSocketException, "WebSocketObserver failed to connect to address " + address);
    }
}

WebSocketObserver::~WebSocketObserver()
{
}

void WebSocketObserver::flush_after_write()
{
    DictObserver::flush_after_write();
    socket->send_text(ss.str());
    ss.str(std::string());
}
