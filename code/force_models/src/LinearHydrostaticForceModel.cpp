/*
 * LinearHydrostaticForceModel.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "LinearHydrostaticForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"

#include <ssc/kinematics.hpp>

const std::string LinearHydrostaticForceModel::model_name = "linear hydrostatics";

LinearHydrostaticForceModel::Input::Input() :
            z_eq(),
            theta_eq(),
            phi_eq(),
            K1(),
            K2(),
            K3(),
            x1(),
            y1(),
            x2(),
            y2(),
            x3(),
            y3(),
            x4(),
            y4()
{
}

LinearHydrostaticForceModel::LinearHydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env) : ForceModel(model_name, body_name_)
{}

ssc::kinematics::Wrench LinearHydrostaticForceModel::operator()(const BodyStates& states, const double) const
{
    return ssc::kinematics::Wrench(states.G,
                                   Eigen::Vector3d(0,0,0),
                                   Eigen::Vector3d(0,0,0));
}
