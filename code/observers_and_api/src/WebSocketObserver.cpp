#include <ssc/websocket.hpp>
#include "base91.hpp"
#include "WebSocketObserver.hpp"

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

/**
 * \brief generate a yaml string with data encoded in base91
 * \code
   # Python 3
   import yaml
   message = "waves: {z: \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,./:;<=>?@[]^_`{|}~\"}"
   yaml.load(message)
   \encode
 */
std::function<void()> WebSocketObserver::get_serializer(const SurfaceElevationGrid& s, const DataAddressing&)
{
    return [this,s](){
        const size_t nx = (size_t)s.x.size();
        const size_t ny = (size_t)s.y.size();
        const size_t n = (size_t)s.z.size();
        std::vector<float> v(n,0.0);
        double const * const data = s.z.data();
        for (size_t i=0;i<n;++i) v[i] = (float)data[i];
        ss<<"waves: {nx : " << nx <<"ny : " << ny << "z: '"<<base<91>::encode(sizeof(float)*n,&v[0])<<"'}";
    };
}

std::function<void()> WebSocketObserver::get_initializer(const SurfaceElevationGrid&, const DataAddressing&)
{
    return [this](){};
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
