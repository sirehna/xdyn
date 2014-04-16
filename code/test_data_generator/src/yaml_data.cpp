/*
 * yaml_data.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "yaml_data.hpp"
#include <sstream>

std::string test_data::rotation()
{
    std::stringstream ss;
    ss << "rotations:" << std::endl
       << "    order by: angle" << std::endl
       << "    convention: [x,y',z'']";
    return ss.str();
}
