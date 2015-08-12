/*
 * LinearHydrostaticForceModel.hpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#ifndef LINEARHYDROSTATICFORCEMODEL_HPP_
#define LINEARHYDROSTATICFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include <ssc/kinematics.hpp>

class LinearHydrostaticForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            double z_eq;
            double theta_eq;
            double phi_eq;
            std::vector<double> K1;
            std::vector<double> K2;
            std::vector<double> K3;
            double x1;
            double y1;
            double x2;
            double y2;
            double x3;
            double y3;
            double x4;
            double y4;
        };
        LinearHydrostaticForceModel(const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        static Input parse(const std::string& yaml);
        static const std::string model_name;

    private:
        LinearHydrostaticForceModel();
        Eigen::Matrix<double,3,3> K;
};

#endif /* GRAVITYFORCEMODEL_HPP_ */
