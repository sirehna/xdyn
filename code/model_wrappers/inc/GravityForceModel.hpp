/*
 * GravityForceModel.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef GRAVITYFORCEMODEL_HPP_
#define GRAVITYFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include <ssc/kinematics.hpp>

class Body;
class EnvironmentAndFrames;

class GravityForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            Input(const EnvironmentAndFrames& env);
            double g;
            ssc::kinematics::KinematicsPtr k;
        };
        GravityForceModel(const Input& in);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;

    private:
        GravityForceModel();
        GravityForceModel(const double g);
        GravityForceModel(const double g, ssc::kinematics::KinematicsPtr k);
        double g;
        ssc::kinematics::KinematicsPtr k;
};

#endif /* GRAVITYFORCEMODEL_HPP_ */
