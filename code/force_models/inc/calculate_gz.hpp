/*
 * GZComputer.hpp
 *
 *  Created on: Mar 26, 2015
 *      Author: cady
 */

#ifndef GZCOMPUTER_HPP_
#define GZCOMPUTER_HPP_

#include <ssc/kinematics.hpp>

double calculate_gz(const ssc::kinematics::Transform& body2ned, const ssc::kinematics::Wrench& force_and_torque_projected_in_NED_frame);

#endif /* GZCOMPUTER_HPP_ */
