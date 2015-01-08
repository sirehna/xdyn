/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlBody.hpp"

Body::Body(const size_t i) : states(), idx(i)
{
}

void Body::update_intersection_with_free_surface(const EnvironmentAndFrames& env,
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

