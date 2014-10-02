/*
 * DampingForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#ifndef DampingForceModel_HPP_
#define DampingForceModel_HPP_

#include "ForceModel.hpp"
#include <Eigen/Dense>
#include <ssc/kinematics.hpp>

class Body;

class DampingForceModel : public ForceModel
{
    public:
        DampingForceModel(const Eigen::Matrix<double,6,6>& D);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;

    private:
        DampingForceModel();
        Eigen::Matrix<double,6,6> D; //!< 6x6 matrix corresponding to the quadratic damping matrix expressed in the body frame
};

#endif /* DampingForceModel_HPP_ */
