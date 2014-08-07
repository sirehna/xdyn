/*
 * HydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "HydrostaticForceModel.hpp"
#include "WaveModelInterface.hpp"
#include "Body.hpp"
#include "hydrostatic.hpp"
#include "EnvironmentAndFrames.hpp"

HydrostaticForceModel::Input::Input() : rho(0),
                                        g(0),
                                        w(WaveModelPtr()),
                                        k(KinematicsPtr())
{
}

HydrostaticForceModel::Input::Input(const EnvironmentAndFrames& env) : rho(env.rho),
                                                                       g(env.g),
                                                                       w(env.w),
                                                                       k(env.k)
{
}

HydrostaticForceModel::HydrostaticForceModel(const Input& in) : rho(in.rho), g(in.g), w(in.w), k(in.k)
{}

Wrench HydrostaticForceModel::operator()(const Body& body, const double t) const
{
    const std::vector<double> dz = w->get_relative_wave_height(*body.M,k,t);
    const auto F = hydrostatic::force(body.mesh, body.G, rho, g, dz);
    return F;
}
