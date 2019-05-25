/*
 * BodyWithSurfaceForces.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: cady
 */

#include "BodyWithSurfaceForces.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"

BodyWithSurfaceForces::BodyWithSurfaceForces(
    const size_t i,
    const BlockedDOF& blocked_states_) :
        Body(i, blocked_states_)
{
}

BodyWithSurfaceForces::BodyWithSurfaceForces(
    const BodyStates& s,
    const size_t i,
    const BlockedDOF& blocked_states_) :
        Body(s, i, blocked_states_)
{
}

void BodyWithSurfaceForces::update_intersection_with_free_surface(
    const EnvironmentAndFrames& env,
    const double t)
{
    if (env.w.use_count())
    {
        env.w->update_surface_elevation(states.M, env.k,t);
        const std::vector<double> dz = env.w->get_relative_wave_height();
        states.intersector->update_intersection_with_free_surface(env.w->get_relative_wave_height(),
                                                                  env.w->get_surface_elevation());
    }
}
