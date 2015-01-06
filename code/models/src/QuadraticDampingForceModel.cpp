/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "QuadraticDampingForceModel.hpp"


QuadraticDampingForceModel::QuadraticDampingForceModel(const Eigen::Matrix<double,6,6>& D_) : DampingForceModel("quadratic damping", D_)
{
}

Eigen::Matrix<double, 6, 1> QuadraticDampingForceModel::get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const
{
    return (Eigen::Matrix<double, 6, 1>)(-D * (Eigen::Matrix<double, 6, 1>)(W.cwiseAbs().array() * W.array()));
}
