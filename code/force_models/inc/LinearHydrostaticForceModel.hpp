/*
 * LinearHydrostaticForceModel.hpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#ifndef LINEARHYDROSTATICFORCEMODEL_HPP_
#define LINEARHYDROSTATICFORCEMODEL_HPP_

#include "EnvironmentAndFrames.hpp"
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
        LinearHydrostaticForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        static Input parse(const std::string& yaml);
        static const std::string model_name;

    private:
        LinearHydrostaticForceModel();
        std::vector<double> get_zH(const double t) const;
        double compute_zbar(const std::vector<double>& z) const;
        double compute_thetabar(const std::vector<double>& z) const;
        double compute_phibar(const std::vector<double>& z) const;
        Eigen::Matrix<double,3,3> K;
        ssc::kinematics::Point P1;
        ssc::kinematics::Point P2;
        ssc::kinematics::Point P3;
        ssc::kinematics::Point P4;
        double z_eq;
        double theta_eq;
        double phi_eq;
        EnvironmentAndFrames env;
        double d12;
        double d34;
        double d13;
        double d24;
};

#endif /* GRAVITYFORCEMODEL_HPP_ */
