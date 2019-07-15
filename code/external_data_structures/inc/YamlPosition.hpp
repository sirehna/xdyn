/*
 * YamlPosition.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLPOSITION_HPP_
#define YAMLPOSITION_HPP_

#include "YamlCoordinates.hpp"
#include "YamlAngle.hpp"

#include <ostream>
#include <string>

struct YamlPosition
{
    YamlPosition();
    YamlPosition(const YamlCoordinates& c, const YamlAngle& a, const std::string& frame);
    YamlCoordinates coordinates;
    YamlAngle angle;
    std::string frame;
};

std::ostream& operator<<(std::ostream& os, const YamlPosition& w);

#endif /* YAMLPOSITION_HPP_ */
