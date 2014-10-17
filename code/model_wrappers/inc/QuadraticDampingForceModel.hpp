/*
 * QuadraticDampingForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#ifndef QuadraticDampingForceModel_HPP_
#define QuadraticDampingForceModel_HPP_

#include "ForceModel.hpp"
#include <Eigen/Dense>
#include <ssc/kinematics.hpp>

class Body;

class QuadraticDampingForceModel : public ForceModel
{
    public:
        QuadraticDampingForceModel(const Eigen::Matrix<double,6,6>& D);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;

    private:
        QuadraticDampingForceModel();
        Eigen::Matrix<double,6,6> D; //!< 6x6 matrix corresponding to the quadratic damping matrix expressed in the body frame
};

#endif /* QuadraticDampingForceModel_HPP_ */
