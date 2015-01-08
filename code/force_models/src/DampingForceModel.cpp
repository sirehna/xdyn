/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include "DampingForceModel.hpp"

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "QuadraticDampingForceModel.hpp"


DampingForceModel::DampingForceModel(const std::string& name_, const Eigen::Matrix<double,6,6>& D_) : ForceModel(name_), D(D_)
{
}

ssc::kinematics::Wrench DampingForceModel::operator()(const Body& body, const double) const
{
    Eigen::Matrix<double, 6, 1> W;
    W <<body.u,
        body.v,
        body.w,
        body.p,
        body.q,
        body.r;
    return ssc::kinematics::Wrench(body.hydrodynamic_forces_calculation_point, get_force_and_torque(D, W));
}

