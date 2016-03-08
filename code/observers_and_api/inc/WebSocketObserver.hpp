#ifndef WEBSOCKETOBSERVER_HPP_
#define WEBSOCKETOBSERVER_HPP_

#include "DictObserver.hpp"

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <vector>
#include <string>

namespace ssc
{
    namespace websocket
    {
        class Client;
    }
}

typedef TR1(shared_ptr)<ssc::websocket::Client> WebSocketPtr;

class WebSocketObserver : public DictObserver
{
    public:
        WebSocketObserver(const std::string& address, const short unsigned int port, const std::vector<std::string>& data);
        ~WebSocketObserver();

    private:
        WebSocketPtr socket;
        void flush_after_write();
};

#endif /* WEBSOCKETOBSERVER_HPP_ */
