/*
 * QuadraticDampingForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#ifndef QuadraticDampingForceModel_HPP_
#define QuadraticDampingForceModel_HPP_

#include "DampingForceModel.hpp"

class QuadraticDampingForceModel : public DampingForceModel
{
    public:
        QuadraticDampingForceModel(const Eigen::Matrix<double,6,6>& D);

    private:
        QuadraticDampingForceModel();
        Eigen::Matrix<double, 6, 1> get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const;
};

#endif /* QuadraticDampingForceModel_HPP_ */
