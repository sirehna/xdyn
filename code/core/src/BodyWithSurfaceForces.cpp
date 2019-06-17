/*
 * BodyWithSurfaceForces.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: cady
 */
#include <ssc/exception_handling.hpp>

#include "BodyWithSurfaceForces.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"

BodyWithSurfaceForces::BodyWithSurfaceForces(const size_t i, const BlockedDOF& blocked_states_) : Body(i, blocked_states_)
{
}

BodyWithSurfaceForces::BodyWithSurfaceForces(const BodyStates& s, const size_t i, const BlockedDOF& blocked_states_) : Body(s, i, blocked_states_)
{
}

void BodyWithSurfaceForces::update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                    const double t)
{
    if (env.w.use_count())
    {
        try
        {
            env.w->update_surface_elevation(states.M, env.k,t);
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses surface force models (eg. Froude-Krylov) which are integrated on the hull. This requires computing the intersection between the hull and the free surface and hence calculating the wave heights. While calculating these wave heights, " << e.get_message());
        }
        const std::vector<double> dz = env.w->get_relative_wave_height();
        states.intersector->update_intersection_with_free_surface(env.w->get_relative_wave_height(),
                                                                  env.w->get_surface_elevation());
    }
}
