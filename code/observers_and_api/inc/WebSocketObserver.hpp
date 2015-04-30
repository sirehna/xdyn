#ifndef WEBSOCKETOBSERVER_HPP_
#define WEBSOCKETOBSERVER_HPP_

#include "Observer.hpp"
#include <ssc/macros.hpp>
#include TR1INC(memory)

class WebSocketEndpoint;
typedef TR1(shared_ptr)<WebSocketEndpoint> WebSocketEndpointPtr;

class WebSocketObserver : public Observer
{
    public:
        WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data);
        ~WebSocketObserver();
        void send(const std::string& message);
        void send(const std::vector<double>& vector);

    private:
        WebSocketEndpointPtr endpoint;
        void flush_after_initialization();
        void flush_after_write();
        void flush_value();
        std::function<void()> get_serializer(const double val, const DataAddressing&);
        std::function<void()> get_initializer(const double, const DataAddressing& address);
        std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing&);
        std::function<void()> get_initializer(const SurfaceElevationGrid&, const DataAddressing& address);
};

#endif /* WEBSOCKETOBSERVER_HPP_ */
