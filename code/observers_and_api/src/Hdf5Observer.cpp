/*
 * Hdf5Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "Hdf5Observer.hpp"

Hdf5Observer::Hdf5Observer(const std::string& , const std::vector<std::string>& d) : Observer(d)
{
}

std::function<void()> Hdf5Observer::get_serializer(const double, const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::string&, const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::vector<std::vector<double> >& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const double , const std::vector<std::string>& , const std::string&)
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::string& , const std::vector<std::string>& , const std::string&)
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::vector<double>& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::vector<std::vector<double> >& , const std::vector<std::string>& , const std::string& )
{
    return [](){};
}

void Hdf5Observer::flush_after_initialization()
{
}

void Hdf5Observer::flush_after_write()
{
}

void Hdf5Observer::flush_value()
{
}
