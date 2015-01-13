/*
 * CSVObserver.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CSVObserver.hpp"

CSVObserver::CSVObserver(const std::string& filename, const std::vector<std::string>& d) : Observer(d), add_coma(false), os(filename)
{
}

std::function<void()> CSVObserver::get_serializer(const double val, const std::vector<std::string>& , const std::string& )
{
    return [this,val](){os << val;};
}

std::function<void()> CSVObserver::get_serializer(const std::string& val, const std::vector<std::string>& , const std::string& )
{
    return [this,val](){os << val;};
}

std::function<void()> CSVObserver::get_serializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> CSVObserver::get_initializer(const double , const std::vector<std::string>& , const std::string& name)
{
    return [this,name](){os << name;};
}

std::function<void()> CSVObserver::get_initializer(const std::string& , const std::vector<std::string>& , const std::string& name)
{
    return [this,name](){os << name;};
}

std::function<void()> CSVObserver::get_initializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

void CSVObserver::flush_after_initialization()
{
    os << std::endl;
}

void CSVObserver::flush_after_write()
{
    os << std::endl;
}

void CSVObserver::flush_value()
{
    os << ',';
}
