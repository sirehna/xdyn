#ifndef JSONOBSERVER_HPP_
#define JSONOBSERVER_HPP_

#include <ostream>
#include <fstream>

#include "Observer.hpp"

#include <map>
#include <string>
typedef std::map<std::string, std::map<std::string, double> > JsonMap;

class JsonObserver : public Observer
{
    public:
        JsonObserver(const std::string& filename, const std::vector<std::string>& d);
        ~JsonObserver();

    private:
        void flush_after_initialization();
        void flush_after_write();
        void flush_value_during_write();

        bool output_to_file;
        std::ostream& os;
        JsonMap jsonMap;

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address);
        std::function<void()> get_initializer(const double val, const DataAddressing& address);
};

#endif
