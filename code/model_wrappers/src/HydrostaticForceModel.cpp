/*
 * HydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "HydrostaticForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"
#include "hydrostatic.hpp"
#include "EnvironmentAndFrames.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"

HydrostaticForceModel::Input::Input() : rho(0),
                                        g(0),
                                        w(SurfaceElevationPtr()),
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
    const Point g_in_NED("NED", 0, 0, g);
    const RotationMatrix ned2mesh = k->get("NED", std::string("mesh(") + body.name + ")").get_rot();

    MeshIntersectorPtr intersector(new MeshIntersector(body.mesh,dz));
    intersector->update_intersection_with_free_surface();

    const auto F = hydrostatic::force((const_MeshIntersectorPtr) intersector, body.G, rho, ned2mesh*g_in_NED.v);
    return F;
}
