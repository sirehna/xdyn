/*
 * YamlDynamics.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlDynamics.hpp"

YamlDynamics::YamlDynamics() :
    centre_of_inertia(YamlPoint()),
    mass(0),
    rigid_body_inertia(YamlInertiaMatrix()),
    added_mass(YamlInertiaMatrix())
{

}
