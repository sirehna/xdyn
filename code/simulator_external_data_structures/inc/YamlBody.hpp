/*
 * YamlBody.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLBODY_HPP_
#define YAMLBODY_HPP_

#include <string>
#include "YamlPosition.hpp"
#include "YamlSpeed.hpp"
#include "YamlDynamics.hpp"
#include "YamlModel.hpp"

struct YamlBody
{
    YamlBody();
    std::string name;
    std::string mesh;
    YamlPosition position_of_body_frame_relative_to_mesh;
    YamlPosition initial_position_of_body_frame_relative_to_NED_projected_in_NED;
    YamlSpeed initial_velocity_of_body_frame_relative_to_NED_projected_in_body;
    YamlDynamics dynamics;
    std::vector<YamlModel> external_forces;
};

#endif /* YAMLBODY_HPP_ */
