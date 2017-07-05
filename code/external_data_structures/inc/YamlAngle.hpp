/*
 * YamlAngle.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLANGLE_HPP_
#define YAMLANGLE_HPP_

struct YamlAngle
{
    YamlAngle();
    YamlAngle(const double phi, const double theta, const double psi);
    double phi;
    double theta;
    double psi;
};

#endif /* YAMLANGLE_HPP_ */
