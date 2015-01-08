/*
 * update_kinematics.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef UPDATE_KINEMATICS_HPP_
#define UPDATE_KINEMATICS_HPP_

#include <ssc/kinematics.hpp>
#include "StateMacros.hpp"

class Body;
struct YamlBody;
struct YamlRotation;

#include <ssc/kinematics.hpp>
#include TR1INC(memory)

typedef TR1(shared_ptr)<ssc::kinematics::Kinematics> KinematicsPtr;

StateType get_initial_states(const YamlRotation& convention, const std::vector<YamlBody>& bodies);

#endif /* UPDATE_KINEMATICS_HPP_ */
