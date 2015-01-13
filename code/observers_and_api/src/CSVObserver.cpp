/*
 * CSVObserver.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CSVObserver.hpp"

CSVObserver::CSVObserver(std::ostream& os_, const std::vector<std::string>& d) : Observer(d), add_coma(false), os(os_)
{
}

std::function<void()> CSVObserver::get_serializer(const double val, const std::vector<std::string>& , const std::string& )
{
    if (should_add_coma()) return [this,val](){os << "," << val;};
                           return [this,val](){os << val;};
}

std::function<void()> CSVObserver::get_serializer(const std::string& val, const std::vector<std::string>& , const std::string& )
{
    if (should_add_coma()) return [this,val](){os << "," << val;};
                           return [this,val](){os << val;};
}

std::function<void()> CSVObserver::get_serializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> CSVObserver::get_initializer(const double , const std::vector<std::string>& , const std::string& name)
{
    if (should_add_coma()) return [this,name](){os << "," << name;};
                           return [this,name](){os << name;};
}

std::function<void()> CSVObserver::get_initializer(const std::string& , const std::vector<std::string>& , const std::string& name)
{
    if (should_add_coma()) return [this,name](){os << "," << name;};
                           return [this,name](){os << name;};
}

std::function<void()> CSVObserver::get_initializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

bool CSVObserver::should_add_coma()
{
    const bool ret = add_coma;
    add_coma = true;
    return ret;
}

void CSVObserver::flush_after_initialization()
{
    add_coma = false;
}

void CSVObserver::flush_after_write()
{
    add_coma = false;
}
