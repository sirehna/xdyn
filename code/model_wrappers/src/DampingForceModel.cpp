/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "QuadraticDampingForceModel.hpp"
#include "YamlDynamics6x6Matrix.hpp"


QuadraticDampingForceModel::QuadraticDampingForceModel(const Eigen::Matrix<double,6,6>& D_) : D(D_)
{
}

ssc::kinematics::Wrench QuadraticDampingForceModel::operator()(const Body& body, const double) const
{
    Eigen::Matrix<double, 6, 1> W;
    W <<body.u,
        body.v,
        body.w,
        body.p,
        body.q,
        body.r;
    W = (W.cwiseAbs().array() * W.array());
    W = -D * W;
    return ssc::kinematics::Wrench(body.G, W);
}

