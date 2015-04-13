/*
 * MapObserver.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: cady
 */

#include "MapObserver.hpp"

MapObserver::MapObserver(const std::vector<std::string>& d) : Observer(d), m()
{
}

std::function<void()> MapObserver::get_serializer(const double val, const DataAddressing& address)
{
    return [this,address,val](){m[address.name].push_back(val);};
}

std::function<void()> MapObserver::get_initializer(const double, const DataAddressing& address)
{
    return [this,address](){m[address.name] = std::vector<double>();};
}

void MapObserver::flush_after_initialization()
{
}

void MapObserver::flush_after_write()
{
}

void MapObserver::flush_value()
{
}

std::map<std::string,std::vector<double> > MapObserver::get() const
{
    return m;
}
