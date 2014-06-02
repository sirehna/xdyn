/*
 * make_stl_file_on_the_fly.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: cady
 */

#include "make_stl_file_on_the_fly.hpp"
#include <sys/stat.h> // For stat
#include <fstream> // For ofstream

inline bool file_exists(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

void make_stl_file(const std::string& data, const std::string& filename)
{
    if (not(file_exists(filename)))
    {
        std::ofstream file;
        file.open(filename.c_str());
        file << data;
    }
}
