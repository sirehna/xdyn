/*
 * YamlDynamics.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLDYNAMICS_HPP_
#define YAMLDYNAMICS_HPP_

#include "YamlPoint.hpp"
#include "YamlInertiaMatrix.hpp"

struct YamlDynamics
{
    YamlDynamics();
    YamlPoint centre_of_inertia;
    double mass;
    YamlInertiaMatrix rigid_body_inertia;
    YamlInertiaMatrix added_mass;
};

#endif /* YAMLDYNAMICS_HPP_ */
