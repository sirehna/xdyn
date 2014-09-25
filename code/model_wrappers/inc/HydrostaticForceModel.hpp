/*
 * HydrostaticForceModel.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICFORCEMODEL_HPP_
#define HYDROSTATICFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include <ssc/kinematics.hpp>
#include "EnvironmentAndFrames.hpp"

class SurfaceElevationInterface;
typedef TR1(shared_ptr)<SurfaceElevationInterface> SurfaceElevationPtr;

class Body;
typedef TR1(shared_ptr)<ssc::kinematics::Kinematics> KinematicsPtr;
class HydrostaticForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            Input(const EnvironmentAndFrames& env);
            double rho;
            double g;
            SurfaceElevationPtr w;
            KinematicsPtr k;
        };

        HydrostaticForceModel(const Input& input);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;

    private:
        HydrostaticForceModel();
        double rho;
        double g;
        SurfaceElevationPtr w;
        KinematicsPtr k;
};

#endif /* HYDROSTATICFORCEMODEL_HPP_ */
