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

GravityForceModel::GravityForceModel(const double g_) : g(g_)
{}

Wrench GravityForceModel::operator()(const Body& body) const
{
    return Wrench(body.G,
                  Eigen::Vector3d(0,0,body.m*g),
                  Eigen::Vector3d(0,0,0));
}

