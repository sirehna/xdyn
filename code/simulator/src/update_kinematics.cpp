/*
 * update_kinematics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"

Point get_origin(const StateType& x, const size_t i)
{
    return Point("NED", *_X(x,i),
                        *_Y(x,i),
                        *_Z(x,i));
}


RotationMatrix get_rot_from_ned_to(const StateType& x, const size_t i)
{
    const Eigen::Quaternion<double> q(*_QR(x,i),
                                      *_QI(x,i),
                                      *_QJ(x,i),
                                      *_QK(x,i));
    return q.matrix();
}
