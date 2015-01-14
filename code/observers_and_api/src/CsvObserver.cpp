/*
 * CsvObserver.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CsvObserver.hpp"

CsvObserver::CsvObserver(const std::string& filename, const std::vector<std::string>& d) : Observer(d), add_coma(false), os(filename)
{
}

std::function<void()> CsvObserver::get_serializer(const double val, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [this,val](){os << val;};
}

std::function<void()> CsvObserver::get_serializer(const std::string& val, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [this,val](){os << val;};
}

std::function<void()> CsvObserver::get_serializer(const std::vector<double>&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> CsvObserver::get_serializer(const std::vector<std::vector<double> >&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> CsvObserver::get_initializer(const double , const std::vector<std::string>&, const std::vector<std::string>&, const std::string& name)
{
    return [this,name](){os << name;};
}

std::function<void()> CsvObserver::get_initializer(const std::string&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& name)
{
    return [this,name](){os << name;};
}

std::function<void()> CsvObserver::get_initializer(const std::vector<double>&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> CsvObserver::get_initializer(const std::vector<std::vector<double> >&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

void CsvObserver::flush_after_initialization()
{
    os << std::endl;
}

void CsvObserver::flush_after_write()
{
    os << std::endl;
}

void CsvObserver::flush_value()
{
    os << ',';
}
