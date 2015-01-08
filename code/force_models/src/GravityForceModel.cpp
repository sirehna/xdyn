/*
 * GravityForceModel.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "GravityForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"

#include <ssc/kinematics.hpp>

const std::string GravityForceModel::model_name = "gravity";

GravityForceModel::GravityForceModel(const EnvironmentAndFrames& env) : ForceModel("gravity"), g(env.g), k(env.k)
{}

ssc::kinematics::Wrench GravityForceModel::operator()(const BodyStates& states, const double) const
{
    const ssc::kinematics::Transform T = k->get(states.name, "NED");
    return ssc::kinematics::Wrench(states.G,
                                   T.get_rot()*Eigen::Vector3d(0,0,states.m*g),
                                   Eigen::Vector3d(0,0,0));
}

double GravityForceModel::potential_energy(const BodyStates& states, const std::vector<double>& x) const
{
    return -states.m*g*x[2];
}
