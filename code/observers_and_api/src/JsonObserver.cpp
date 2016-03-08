#include "JsonObserver.hpp"
#include <iostream>
#include <fstream>

JsonObserver::JsonObserver(
        const std::string& filename, const std::vector<std::string>& d) :
        DictObserver(d),
        output_to_file(not(filename.empty())),
        os(output_to_file ? *(new std::ofstream(filename)) : std::cout)
{
}

JsonObserver::~JsonObserver()
{
    if (output_to_file) delete(&os);
}

void JsonObserver::flush_after_write()
{
    DictObserver::flush_after_write();
    os << ss.str() << std::endl << std::flush;
    ss.str(std::string());
}

