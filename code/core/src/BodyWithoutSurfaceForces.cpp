/*
 * BodyWithoutSurfaceForces.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: cady
 */

#include "BodyWithoutSurfaceForces.hpp"

BodyWithoutSurfaceForces::BodyWithoutSurfaceForces(const size_t i) : Body(i)
{
}

void BodyWithoutSurfaceForces::update_intersection_with_free_surface(const EnvironmentAndFrames& ,
                                    const double )
{
}
