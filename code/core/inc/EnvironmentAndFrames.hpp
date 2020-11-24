/*
 * EnvironmentAndFrames.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENTANDFRAMES_HPP_
#define ENVIRONMENTANDFRAMES_HPP_

#include "YamlRotation.hpp"
#include "Body.hpp"
#include "StateMacros.hpp"
#include "SurfaceElevationInterface.hpp"
#include "WindModel.hpp"
#include <ssc/kinematics.hpp>

class Observer;

struct EnvironmentAndFrames
{
    EnvironmentAndFrames();
    void feed(Observer& observer, double t, const std::vector<BodyPtr>& bodies, const StateType& state) const;
    SurfaceElevationPtr w;
    WindModelPtr wind;
    ssc::kinematics::KinematicsPtr k;
    double rho;
    double nu;
    double g;
    YamlRotation rot;
};

#endif /* ENVIRONMENTANDFRAMES_HPP_ */
