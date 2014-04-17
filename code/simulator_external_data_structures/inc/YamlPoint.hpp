/*
 * YamlPoint.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLPOINT_HPP_
#define YAMLPOINT_HPP_

#include <string>

#include "YamlCoordinates.hpp"

struct YamlPoint : public YamlCoordinates
{
    YamlPoint();
    YamlPoint(const YamlCoordinates& c);
    std::string name;
    std::string frame;
};

#endif /* YAMLPOINT_HPP_ */
