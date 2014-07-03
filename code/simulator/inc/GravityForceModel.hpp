/*
 * GravityForceModel.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef GRAVITYFORCEMODEL_HPP_
#define GRAVITYFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include "Kinematics.hpp"

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
            KinematicsPtr k;
        };
        GravityForceModel(const Input& in);
        Wrench operator()(const Body& body) const;

    private:
        GravityForceModel(const double g);
        GravityForceModel(const double g, KinematicsPtr k);
        GravityForceModel();
        double g;
        KinematicsPtr k;
};


#endif /* GRAVITYFORCEMODEL_HPP_ */
