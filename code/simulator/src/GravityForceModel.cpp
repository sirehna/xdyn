/*
 * GravityForceModel.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "GravityForceModel.hpp"
#include "WaveModelInterface.hpp"
#include "Body.hpp"
#include "hydrostatic.hpp"
#include "EnvironmentAndFrames.hpp"

GravityForceModel::Input::Input() : g(0)
{
}

GravityForceModel::Input::Input(const EnvironmentAndFrames& env) : g(env.g)
{
}

GravityForceModel::GravityForceModel(const Input& in) : g(in.g)
{}

Wrench GravityForceModel::operator()(const Body& body) const
{
    return Wrench(body.G,
                  Eigen::Vector3d(0,0,body.m*g),
                  Eigen::Vector3d(0,0,0));
}

