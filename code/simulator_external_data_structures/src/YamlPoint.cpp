/*
 * YamlPoint.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlPoint.hpp"

YamlPoint::YamlPoint() : YamlCoordinates(), name(""), frame("")
{

}

YamlPoint::YamlPoint(const YamlCoordinates& c) : YamlCoordinates(c), name(""), frame("")
{

}
