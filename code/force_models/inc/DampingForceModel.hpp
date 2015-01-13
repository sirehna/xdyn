/*
 * DampingForceModel.hpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#ifndef DAMPINGFORCEMODEL_HPP_
#define DAMPINGFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include <Eigen/Dense>
#include <ssc/kinematics.hpp>

class Body;

/** \brief Provides an interface to QuadraticDampingForceModel & LinearDampingForceModel
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 */
class DampingForceModel : public ForceModel
{
    public:
        DampingForceModel(const std::string& name, const std::string& body_name, const Eigen::Matrix<double,6,6>& D);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;

    private:
        virtual Eigen::Matrix<double, 6, 1> get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const = 0;
        DampingForceModel();
        Eigen::Matrix<double,6,6> D; //!< 6x6 matrix corresponding to the quadratic damping matrix expressed in the body frame
};

#endif /* DAMPINGFORCEMODEL_HPP_ */
