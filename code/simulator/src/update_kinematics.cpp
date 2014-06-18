/*
 * update_kinematics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"
#include "Body.hpp"
#include "Kinematics.hpp"

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

Point get_position_of_body_relative_to_mesh(const Body& body)
{
    return Point(std::string("mesh(")+body.name+")", body.x_relative_to_mesh,
                                                     body.y_relative_to_mesh,
                                                     body.z_relative_to_mesh);
}

kinematics::Transform get_transform_from_mesh_to(const Body& body)
{
    return kinematics::Transform(get_position_of_body_relative_to_mesh(body), body.mesh_to_body, body.name);
}

kinematics::Transform get_transform_from_ned_to(const StateType& x, const Body& body, const size_t idx)
{
    return kinematics::Transform(get_origin(x, idx), get_rot_from_ned_to(x, idx), body.name);
}

void update_kinematics(const StateType& x, const Body& body, const size_t idx, const KinematicsPtr& k)
{
    k->add(get_transform_from_ned_to(x,body,idx));
}
