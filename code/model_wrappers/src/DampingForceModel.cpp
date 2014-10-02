/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "DampingForceModel.hpp"

ssc::kinematics::Wrench DampingForceModel::operator()(const Body& body, const double) const
{
    Eigen::Matrix<double, 6, 1> W;
    W <<body.u,
        body.v,
        body.w,
        body.p,
        body.q,
        body.r;
    W = (W.cwiseAbs().array() * W.array());
    W = (*body.damping) * W;
    return ssc::kinematics::Wrench(body.G,
                                   Eigen::Vector3d(W(0,0),W(1,0),W(2,0)),
                                   Eigen::Vector3d(W(3,0),W(4,0),W(5,0)));
}

