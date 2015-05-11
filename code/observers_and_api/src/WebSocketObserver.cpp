#include "WebSocketObserver.hpp"
#include <ssc/websocket.hpp>

WebSocketObserver::WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data):
Observer(data),socket(new ssc::websocket::Client(address, port)), ss()
{
    if (not(socket->good()))
    {
        THROW(__PRETTY_FUNCTION__, ssc::websocket::WebSocketException, "WebSocketObserver failed to connect to address " + address);
    }
}

WebSocketObserver::~WebSocketObserver()
{
}

std::function<void()> WebSocketObserver::get_serializer(const double val, const DataAddressing& da)
{
    return [this,val,da]()
           {
              ss << da.name << ": " << val;
           };
}

std::function<void()> WebSocketObserver::get_initializer(const double, const DataAddressing& )
{
    return [](){};
}

void WebSocketObserver::flush_after_initialization()
{
}

void WebSocketObserver::before_write()
{
     ss << "{";
}

void WebSocketObserver::flush_after_write()
{
    ss << "}";
    socket->send_text(ss.str());
    ss.clear();//clear any bits set
    ss.str(std::string());
}

void WebSocketObserver::flush_value_during_write()
{
    ss << ", ";
}

void WebSocketObserver::flush_value_during_initialization()
{
}
