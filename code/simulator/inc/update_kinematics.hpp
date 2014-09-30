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
class YamlBody;
class YamlRotation;

#include <ssc/kinematics.hpp>
#include TR1INC(memory)

typedef TR1(shared_ptr)<ssc::kinematics::Kinematics> KinematicsPtr;

ssc::kinematics::Point get_origin(const StateType& x, const size_t i);
ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x, const size_t body);
ssc::kinematics::Point get_position_of_body_relative_to_mesh(const Body& body);
ssc::kinematics::Transform get_transform_from_mesh_to(const Body& body);
ssc::kinematics::Transform get_transform_from_ned_to(const StateType& x, const Body& body, const size_t idx);
void update_kinematics(StateType x, const Body& body, const size_t idx, const KinematicsPtr& k);
StateType get_initial_states(const YamlRotation& convention, const std::vector<YamlBody>& bodies);
void update_body_states(const StateType& x, Body& body, const size_t idx);

#endif /* UPDATE_KINEMATICS_HPP_ */
