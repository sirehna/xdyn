#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "TsvObserver.hpp"

#define PRECISION 3
#define WIDTH (PRECISION+6)
#include <ssc/macros.hpp>
TsvObserver::TsvObserver(const std::string& filename, const std::vector<std::string>& d) :
            Observer(d),
            output_to_file(not(filename.empty())),
            os(output_to_file ? *(new std::ofstream(filename)) : std::cout),
            length_of_title_line(0)
{
    os << std::scientific
       << std::setw(WIDTH)
       << std::setprecision(PRECISION);
}

TsvObserver::~TsvObserver()
{
    if (output_to_file) delete(&os);
}

std::function<void()> TsvObserver::get_serializer(const double val, const DataAddressing&)
{
    return [this,val](){os << val;};
}

std::function<void()> TsvObserver::get_initializer(const double, const DataAddressing& address)
{
    return [this,address](){length_of_title_line+=std::max((int)address.name.size(),WIDTH)+1;os << std::setw(WIDTH) << address.name;};
}

void TsvObserver::flush_after_initialization()
{
    os << std::endl;
    os << std::string(length_of_title_line-1,'-') << std::endl;
}

void TsvObserver::flush_after_write()
{
    os << std::endl;
}

void TsvObserver::flush_value()
{
    os << ' ';
}
