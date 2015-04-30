#include "WebSocketObserver.hpp"
#include "WebSocketObserverException.hpp"
#include "WebSocketEndpoint.hpp"

WebSocketObserver::WebSocketObserver(const std::string& address, const std::vector<std::string>& data):
Observer(data),endpoint(new WebSocketEndpoint(address))
{
    if (endpoint->good()) std::cout<<"Connection successful" << std::endl << std::flush;
    else
    {
        THROW(__PRETTY_FUNCTION__, WebSocketException, "WebSocketObserver failed to connect to address" + address);
    }
}

void WebSocketObserver::send(const std::string& message)
{
    this->endpoint->send(endpoint->get_current_id(),message);
}

void WebSocketObserver::send(const std::vector<double>& vector)
{
    this->endpoint->send(endpoint->get_current_id(),vector);
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

std::ostream & operator<< (std::ostream & out, WebSocketObserver const & wsObserver)
{
    for (const auto id:wsObserver.endpoint->get_ids())
    {
        connection_metadata::ptr metadata = wsObserver.endpoint->get_metadata(id);
        if (metadata)
        {
            out << *metadata << std::endl;
        }
        else
        {
            std::stringstream ss;
            ss << "Unknown connection id : " <<id<<std::endl;
            THROW(__PRETTY_FUNCTION__, WebSocketException, ss.str());
        }
    }
    return out;
}

