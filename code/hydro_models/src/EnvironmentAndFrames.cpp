/*
 * EnvironmentAndFrames.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include <ssc/kinematics.hpp>

using namespace ssc::kinematics;

EnvironmentAndFrames::EnvironmentAndFrames() : w(SurfaceElevationPtr()),
                                               k(KinematicsPtr(new Kinematics())),
                                               rho(0),
                                               g(0)
{
}
