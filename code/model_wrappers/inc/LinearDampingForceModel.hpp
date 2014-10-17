/*
 * LinearDampingForceModel.hpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#ifndef LINEARDAMPINGFORCEMODEL_HPP_
#define LINEARDAMPINGFORCEMODEL_HPP_

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
        LinearDampingForceModel(const Eigen::Matrix<double,6,6>& D);

    private:
        LinearDampingForceModel();
        Eigen::Matrix<double, 6, 1> get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const;
};

#endif /* LINEARDAMPINGFORCEMODEL_HPP_ */
