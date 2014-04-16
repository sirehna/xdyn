/*
 * YamlBody.cpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#include "YamlBody.hpp"

YamlBody::YamlBody() :
    name(""),
    mesh(""),
    position_of_body_frame_relative_to_mesh(YamlPosition()),
    initial_position_of_body_frame_relative_to_NED_projected_in_NED(YamlPosition()),
    initial_velocity_of_body_frame_relative_to_NED_projected_in_body(YamlSpeed()),
    dynamics(YamlDynamics()),
    external_forces(std::vector<YamlModel>())
{

}
