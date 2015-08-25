/*
 * BodyWithoutSurfaceForces.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: cady
 */

#include "BodyWithoutSurfaceForces.hpp"

BodyWithoutSurfaceForces::BodyWithoutSurfaceForces(const size_t i, const BlockedDOF& blocked_states_) : Body(i, blocked_states_)
{
}

BodyWithoutSurfaceForces::BodyWithoutSurfaceForces(const BodyStates& s, const size_t i, const BlockedDOF& blocked_states_) : Body(s, i, blocked_states_)
{
}

void BodyWithoutSurfaceForces::update_intersection_with_free_surface(const EnvironmentAndFrames& ,
                                    const double )
{
}
