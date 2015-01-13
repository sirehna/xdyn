/*
 * EnvironmentAndFrames.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>

#include "EnvironmentAndFrames.hpp"
#include "Observer.hpp"

using namespace ssc::kinematics;

EnvironmentAndFrames::EnvironmentAndFrames() : w(SurfaceElevationPtr()),
                                               k(KinematicsPtr(new Kinematics())),
                                               rho(0),
                                               g(0),
                                               rot()
{
}

void EnvironmentAndFrames::feed(Observer& observer) const
{
    (void)observer;
}
