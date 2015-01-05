/*
 * YamlSpeed.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLSPEED_HPP_
#define YAMLSPEED_HPP_

#include <string>

struct YamlSpeed
{
    YamlSpeed();
    std::string frame;
    double u;
    double v;
    double w;
    double p;
    double q;
    double r;
};

#endif /* YAMLSPEED_HPP_ */
