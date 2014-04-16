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

std::string test_data::model1()
{
    std::stringstream ss;
    ss << "type: no waves" << std::endl
       << "frame: NED" << std::endl
       << "z: {value: 0, unit: m}" << std::endl;
    return ss.str();
}

std::string test_data::model2()
{
    std::stringstream ss;
    ss << "type: gravity" << std::endl
       << "g: {value: 9.81, unit: m/s^2}" << std::endl;
    return ss.str();
}
