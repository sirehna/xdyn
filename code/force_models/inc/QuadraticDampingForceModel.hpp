/*
 * QuadraticDampingForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#ifndef QuadraticDampingForceModel_HPP_
#define QuadraticDampingForceModel_HPP_

struct EnvironmentAndFrames;

#include "DampingForceModel.hpp"

class QuadraticDampingForceModel : public DampingForceModel
{
    public:
        typedef Eigen::Matrix<double,6,6> Input;
        QuadraticDampingForceModel(const Input& D, const std::string& body_name, const EnvironmentAndFrames& env);
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        QuadraticDampingForceModel();
        Eigen::Matrix<double, 6, 1> get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const;
};

#endif /* QuadraticDampingForceModel_HPP_ */
