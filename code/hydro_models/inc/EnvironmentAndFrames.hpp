/*
 * EnvironmentAndFrames.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENTANDFRAMES_HPP_
#define ENVIRONMENTANDFRAMES_HPP_

#include <ssc/macros/tr1_macros.hpp>
#include TR1INC(memory)

#include <ssc/kinematics.hpp>

class SurfaceElevationInterface;
typedef TR1(shared_ptr)<SurfaceElevationInterface> SurfaceElevationPtr;

struct EnvironmentAndFrames
{
    EnvironmentAndFrames();
    SurfaceElevationPtr w;
    ssc::kinematics::KinematicsPtr k;
    double rho;
    double g;
};

#endif /* ENVIRONMENTANDFRAMES_HPP_ */
