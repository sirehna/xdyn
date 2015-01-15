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

#include "YamlRotation.hpp"

class Observer;
class SurfaceElevationInterface;
typedef TR1(shared_ptr)<SurfaceElevationInterface> SurfaceElevationPtr;

struct EnvironmentAndFrames
{
    EnvironmentAndFrames();
    void feed(Observer& observer, double t) const;
    SurfaceElevationPtr w;
    ssc::kinematics::KinematicsPtr k;
    double rho;
    double g;
    YamlRotation rot;
};

#endif /* ENVIRONMENTANDFRAMES_HPP_ */
