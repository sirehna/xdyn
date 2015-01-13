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
struct EnvironmentAndFrames;

class GravityForceModel : public ForceModel
{
    public:
        GravityForceModel(const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        double potential_energy(const BodyStates& states, const std::vector<double>& x) const;
        static const std::string model_name;

    private:
        GravityForceModel();
        GravityForceModel(const double g);
        double g;
        ssc::kinematics::KinematicsPtr k;
};

#endif /* GRAVITYFORCEMODEL_HPP_ */
