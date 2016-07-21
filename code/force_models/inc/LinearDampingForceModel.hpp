/*
 * LinearDampingForceModel.hpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#ifndef LINEARDAMPINGFORCEMODEL_HPP_
#define LINEARDAMPINGFORCEMODEL_HPP_

struct EnvironmentAndFrames;

/** \brief
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/LinearDampingForceModelTest.cpp LinearDampingForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/LinearDampingForceModelTest.cpp LinearDampingForceModelTest expected output
 */
#include "DampingForceModel.hpp"

class LinearDampingForceModel : public DampingForceModel
{
    public:
        typedef Eigen::Matrix<double,6,6> Input;
        LinearDampingForceModel(const Input& D, const std::string& body_name, const EnvironmentAndFrames& env);
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        LinearDampingForceModel();
        Eigen::Matrix<double, 6, 1> get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const;
};

#endif /* LINEARDAMPINGFORCEMODEL_HPP_ */
