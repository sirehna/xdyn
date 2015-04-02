#include <iostream>
#include "JsonObserver.hpp"

#include <utility>
typedef std::pair<std::string,std::string> JsonMapKeyVar;
JsonMapKeyVar extractKeyVarFromString(const std::string& s);
JsonMapKeyVar extractKeyVarFromString(const std::string& s)
{
    JsonMapKeyVar j;
    auto kS = s.find('(');
    auto kE = s.find_last_of(')');
    if ((kS != std::string::npos) and (kE != std::string::npos))
    {
        j.first = s.substr(kS+1,kE-kS-1);
        j.second = s.substr(0,kS);
    }
    return j;
}

JsonObserver::JsonObserver(
        const std::string& filename, const std::vector<std::string>& d) :
        Observer(d),
        output_to_file(not(filename.empty())),
        os(output_to_file ? *(new std::ofstream(filename)) : std::cout),
        jsonMap()
{
}

JsonObserver::~JsonObserver()
{
    if (output_to_file) delete(&os);
}

std::function<void()> JsonObserver::get_serializer(const double val, const DataAddressing& d)
{
    return [this,d,val]()
                      {
                        JsonMapKeyVar j = extractKeyVarFromString(d.name);
                        if ((not j.first.empty()) and (not j.second.empty()))
                        {
                           jsonMap[j.first][j.second]=val;
                        }
                      };
}

std::function<void()> JsonObserver::get_initializer(const double, const DataAddressing&)
{
    return [this](){};
}

void JsonObserver::flush_after_initialization()
{
}

void flushMap(std::ostream& os, const std::map<std::string,double>& stuff_to_write);
void flushMap(std::ostream& os, const std::map<std::string,double>& stuff_to_write)
{
    const size_t n = stuff_to_write.size();
    size_t i = 0;
    os << "{";
    for (auto const& stuff:stuff_to_write)
    {
        os << "'"<<stuff.first<<"':"<<stuff.second;
        if (i<(n-1)) os << ",";
        ++i;
    }
    os << "}";
}

void JsonObserver::flush_after_write()
{
    os << "[";
    for (auto const& object:jsonMap)
    {
        os << "{";
        os << "'name':'"<<object.first<<"',";
        if (object.first.find(',')==std::string::npos)
        {
            os << "'type':'attitude',";
        }
        else
        {
            os << "'type':'wrench',";
        }
        os << "'var':";
        flushMap(os, object.second);
        os << "}";
    }
    os << "]"<<std::endl;
    os <<std::flush;
}

void JsonObserver::flush_value()
{
}
