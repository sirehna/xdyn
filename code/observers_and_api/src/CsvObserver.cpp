/*
 * CsvObserver.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "CsvObserver.hpp"

CsvObserver::CsvObserver(const std::string& filename, const std::vector<std::string>& d) :
        Observer(d),
        output_to_file(not(filename.empty())),
        os(output_to_file ? *(new std::ofstream(filename)) : std::cout)
{
    os << std::scientific;
}

CsvObserver::~CsvObserver()
{
    if (output_to_file) delete(&os);
}

std::function<void()> CsvObserver::get_serializer(const double val, const DataAddressing&)
{
    return [this,val](){os << val;};
}

std::function<void()> CsvObserver::get_initializer(const double, const DataAddressing& address)
{
    return [this,address](){std::string title = address.name;boost::replace_all(title, ",", " ");os << title;};
}

void CsvObserver::flush_after_initialization()
{
    os << std::endl;
}

void CsvObserver::flush_after_write()
{
    os << std::endl;
}

void CsvObserver::flush_value_during_write()
{
    os << ',';
}
