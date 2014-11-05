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
#include "hydrostatic.hpp"
#include <ssc/kinematics.hpp>

GravityForceModel::Input::Input() : g(0),
                                    k(ssc::kinematics::KinematicsPtr())
{
}

GravityForceModel::Input::Input(const EnvironmentAndFrames& env) : g(env.g),
                                                                   k(env.k)
{
}

GravityForceModel::GravityForceModel(const Input& in) : ForceModel("gravity"), g(in.g), k(in.k)
{}

ssc::kinematics::Wrench GravityForceModel::operator()(const Body& body, const double) const
{
    const ssc::kinematics::Transform T = k->get(body.name, "NED");
    return ssc::kinematics::Wrench(body.G,
                                   T.get_rot()*Eigen::Vector3d(0,0,body.m*g),
                                   Eigen::Vector3d(0,0,0));
}

double GravityForceModel::potential_energy(const Body& body, const std::vector<double>& x) const
{
    return -body.m*g*x[2];
}
