/*
 * LinearDampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "LinearDampingForceModel.hpp"

LinearDampingForceModel::LinearDampingForceModel(const Eigen::Matrix<double,6,6>& D_) : DampingForceModel("linear damping", D_)
{
}

Eigen::Matrix<double, 6, 1> LinearDampingForceModel::get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const
{
    return (Eigen::Matrix<double, 6, 1>)(-D * W);
}
