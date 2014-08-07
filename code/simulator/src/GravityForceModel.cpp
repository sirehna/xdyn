/*
 * GravityForceModel.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "GravityForceModel.hpp"
#include "Body.hpp"
#include "hydrostatic.hpp"
#include "EnvironmentAndFrames.hpp"
#include "Transform.hpp"

GravityForceModel::Input::Input() : g(0),
                                    k(KinematicsPtr())
{
}

GravityForceModel::Input::Input(const EnvironmentAndFrames& env) : g(env.g),
                                                                   k(env.k)
{
}

GravityForceModel::GravityForceModel(const Input& in) : g(in.g), k(in.k)
{}

Wrench GravityForceModel::operator()(const Body& body, const double) const
{
    const kinematics::Transform T = k->get(body.name, "NED");
    return Wrench(body.G,
                  T.get_rot()*Eigen::Vector3d(0,0,body.m*g),
                  Eigen::Vector3d(0,0,0));
}

