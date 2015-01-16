/*
 * CsvObserver.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CsvObserver.hpp"

CsvObserver::CsvObserver(const std::string& filename, const std::vector<std::string>& d) : Observer(d), os(filename)
{
}

std::function<void()> CsvObserver::get_serializer(const double val, const DataAddressing&)
{
    return [this,val](){os << val;};
}

std::function<void()> CsvObserver::get_initializer(const double, const DataAddressing& address)
{
    return [this,address](){os << address.name;};
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
