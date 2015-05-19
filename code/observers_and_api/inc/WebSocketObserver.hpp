#ifndef WEBSOCKETOBSERVER_HPP_
#define WEBSOCKETOBSERVER_HPP_

#include <sstream>

#include "Observer.hpp"
#include <ssc/macros.hpp>
#include TR1INC(memory)

namespace ssc
{
    namespace websocket
    {
        class Client;
    }
}

typedef TR1(shared_ptr)<ssc::websocket::Client> WebSocketPtr;

class WebSocketObserver : public Observer
{
    public:
        WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data);
        ~WebSocketObserver();

    private:
        WebSocketPtr socket;
        void flush_after_initialization();
        void flush_after_write();
        void flush_value_during_write();
        void flush_value_during_initialization();
        void before_write();

        using Observer::get_serializer;
        using Observer::get_initializer;
        std::function<void()> get_serializer(const double val, const DataAddressing&);
        std::function<void()> get_initializer(const double, const DataAddressing& address);

        std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing& address);
        std::function<void()> get_initializer(const SurfaceElevationGrid& val, const DataAddressing& address);

        std::stringstream ss;
};

#endif /* WEBSOCKETOBSERVER_HPP_ */
