/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlBody.hpp"

Body::Body(const size_t i) : states(), idx(i)
{
}

Body::~Body()
{
}

#define SQUARE(x) ((x)*(x))

ssc::kinematics::Point Body::get_origin(const StateType& x) const
{
    return ssc::kinematics::Point("NED", *_X(x,idx),
                        *_Y(x,idx),
                        *_Z(x,idx));
}


ssc::kinematics::RotationMatrix Body::get_rot_from_ned_to(const StateType& x) const
{
    const Eigen::Quaternion<double> q(*_QR(x,idx),
                                      *_QI(x,idx),
                                      *_QJ(x,idx),
                                      *_QK(x,idx));
    return q.matrix();
}

ssc::kinematics::Point Body::get_position_of_body_relative_to_mesh() const
{
    return ssc::kinematics::Point(std::string("mesh(")+states.name+")", states.x_relative_to_mesh,
                                                     states.y_relative_to_mesh,
                                                     states.z_relative_to_mesh);
}

ssc::kinematics::Transform Body::get_transform_from_mesh_to() const
{
    return ssc::kinematics::Transform(get_position_of_body_relative_to_mesh(), states.mesh_to_body, states.name);
}

ssc::kinematics::Transform Body::get_transform_from_ned_to(const StateType& x) const
{
    return ssc::kinematics::Transform(get_origin(x), get_rot_from_ned_to(x), states.name);
}

void Body::update_kinematics(StateType x, const KinematicsPtr& k) const
{
    k->add(get_transform_from_ned_to(x));
}

void Body::update_body_states(const StateType& x)
{
    states.u = *_U(x,idx);
    states.v = *_V(x,idx);
    states.w = *_W(x,idx);
    states.p = *_P(x,idx);
    states.q = *_Q(x,idx);
    states.r = *_R(x,idx);
}

void Body::update_projection_of_z_in_mesh_frame(const double g,
                                                ssc::kinematics::KinematicsPtr& k)
{
    const ssc::kinematics::Point g_in_NED("NED", 0, 0, g);
    const ssc::kinematics::RotationMatrix ned2mesh = k->get("NED", std::string("mesh(") + states.name + ")").get_rot();
    states.down_direction_in_mesh_frame = ned2mesh*g_in_NED.v;
}
