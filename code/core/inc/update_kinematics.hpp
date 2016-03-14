/*
 * update_kinematics.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef UPDATE_KINEMATICS_HPP_
#define UPDATE_KINEMATICS_HPP_

#include <vector>

#include "StateMacros.hpp"

struct YamlBody;
struct YamlRotation;

StateType get_initial_states(const YamlRotation& convention, const std::vector<YamlBody>& bodies);

#endif /* UPDATE_KINEMATICS_HPP_ */
