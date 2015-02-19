/*
 * YamlPosition.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlPosition.hpp"

YamlPosition::YamlPosition() : coordinates(YamlCoordinates()),
                               angle(YamlAngle()),
                               frame("")
{}

YamlPosition::YamlPosition(const YamlCoordinates& c, const YamlAngle& a, const std::string& f) :
        coordinates(c),
        angle(a),
        frame(f)
{
}
