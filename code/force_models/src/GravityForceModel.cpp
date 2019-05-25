/*
 * GravityForceModel.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "GravityForceModel.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"

#include <ssc/kinematics.hpp>

std::string GravityForceModel::model_name(){return "gravity";}

GravityForceModel::GravityForceModel(const std::string& body_name_, const EnvironmentAndFrames& env)
    : ForceModel("gravity", body_name_)
    , g(env.g)
    , k(env.k)
{}

ssc::kinematics::Wrench GravityForceModel::operator()(const BodyStates& states, const double) const
{
    const ssc::kinematics::Transform T = k->get(states.name, "NED");
    const double m = states.solid_body_inertia->operator()(2,2);
    return ssc::kinematics::Wrench(states.G,
                                   T.get_rot()*Eigen::Vector3d(0,0,m*g),
                                   Eigen::Vector3d(0,0,0));
}

double GravityForceModel::potential_energy(const BodyStates& states, const std::vector<double>& x) const
{
    const double m = states.solid_body_inertia->operator()(2,2);
    return -m*g*x[2];
}
