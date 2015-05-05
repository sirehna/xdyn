#include "WebSocketObserver.hpp"
#include "WebSocketException.hpp"
#include "WebSocketClient.hpp"

WebSocketObserver::WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data):
Observer(data),endpoint(new ssc::websocket::Client(address, port))
{
    if (not(endpoint->good()))
    {
        THROW(__PRETTY_FUNCTION__, ssc::websocket::WebSocketException, "WebSocketObserver failed to connect to address " + address);
    }
}

void WebSocketObserver::send(const std::string& message)
{
    endpoint->send_text(message);
}

void WebSocketObserver::send(const std::vector<double>& vector)
{
    endpoint->send_binary(vector);
}

WebSocketObserver::~WebSocketObserver()
{
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

