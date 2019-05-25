/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include "DampingForceModel.hpp"

#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include <Eigen/Dense>


DampingForceModel::DampingForceModel(const std::string& name_, const std::string& body_name_, const Eigen::Matrix<double,6,6>& D_) : ForceModel(name_, body_name_), D(D_)
{
}

ssc::kinematics::Wrench DampingForceModel::operator()(const BodyStates& states, const double) const
{
    Eigen::Matrix<double, 6, 1> W;
    W <<states.u(),
        states.v(),
        states.w(),
        states.p(),
        states.q(),
        states.r();
    return ssc::kinematics::Wrench(states.hydrodynamic_forces_calculation_point, get_force_and_torque(D, W));
}

