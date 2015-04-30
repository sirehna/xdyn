#include "WebSocketObserver.hpp"
#include "WebSocketException.hpp"
#include "WebSocketEndpoint.hpp"

WebSocketObserver::WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data):
Observer(data),endpoint(new WebSocketClient(address, port))
{
    if (endpoint->good()) std::cout<<"Connection successful" << std::endl << std::flush;
    else
    {
        THROW(__PRETTY_FUNCTION__, WebSocketException, "WebSocketObserver failed to connect to address" + address);
    }
}

void WebSocketObserver::send(const std::string& message)
{
    this->endpoint->send(message);
}

void WebSocketObserver::send(const std::vector<double>& vector)
{
    this->endpoint->send(vector);
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

