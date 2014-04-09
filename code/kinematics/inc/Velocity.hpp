/*
 * Velocity.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef VELOCITY_HPP_
#define VELOCITY_HPP_

#include "Point.hpp"

/** \author cec
 *  \brief 
 *  \details 
 *  \ingroup kinematics
 */
struct Velocity
{
    Eigen::Vector3d translation;
    Eigen::Vector3d rotation;
    double& u;
    double& v;
    double& w;
    double& p;
    double& q;
    double& r;
    Point P;
    std::string reference_frame;
    std::string projection_frame;
};

#endif // VELOCITY_HPP_