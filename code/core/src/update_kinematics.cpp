/*
 * update_kinematics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */
#include <ssc/kinematics.hpp>

#include "Body.hpp"
#include "BodyBuilder.hpp"
#include "StateMacros.hpp"
#include "update_kinematics.hpp"
#include "yaml2eigen.hpp"
#include "YamlAngle.hpp"
#include "YamlBody.hpp"

#define SQUARE(x) ((x)*(x))

ssc::kinematics::Point get_origin(const StateType& x, const size_t i)
{
    return ssc::kinematics::Point("NED", *_X(x,i),
                        *_Y(x,i),
                        *_Z(x,i));
}


ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x, const size_t i)
{
    const Eigen::Quaternion<double> q(*_QR(x,i),
                                      *_QI(x,i),
                                      *_QJ(x,i),
                                      *_QK(x,i));
    return q.matrix();
}

ssc::kinematics::Point get_position_of_body_relative_to_mesh(const Body& body)
{
    return ssc::kinematics::Point(std::string("mesh(")+body.name+")", body.x_relative_to_mesh,
                                                     body.y_relative_to_mesh,
                                                     body.z_relative_to_mesh);
}

ssc::kinematics::Transform get_transform_from_mesh_to(const Body& body)
{
    return ssc::kinematics::Transform(get_position_of_body_relative_to_mesh(body), body.mesh_to_body, body.name);
}

ssc::kinematics::Transform get_transform_from_ned_to(const StateType& x, const Body& body, const size_t idx)
{
    return ssc::kinematics::Transform(get_origin(x, idx), get_rot_from_ned_to(x, idx), body.name);
}

void update_kinematics(StateType x, const Body& body, const size_t idx, const KinematicsPtr& k)
{
    k->add(get_transform_from_ned_to(x,body,idx));
}

StateType get_initial_states(const YamlRotation& convention, const std::vector<YamlBody>& bodies)
{
    StateType ret(13*bodies.size(),0);
    BodyBuilder builder(convention);
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        const YamlAngle angle = bodies[i].initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle;
        const ssc::kinematics::RotationMatrix R = angle2matrix(angle, convention);
        const Eigen::Quaternion<double> q(R);
        *_X(ret,i) = bodies[i].initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.x;
        *_Y(ret,i) = bodies[i].initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.y;
        *_Z(ret,i) = bodies[i].initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.z;
        *_U(ret,i) = bodies[i].initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u;
        *_V(ret,i) = bodies[i].initial_velocity_of_body_frame_relative_to_NED_projected_in_body.v;
        *_W(ret,i) = bodies[i].initial_velocity_of_body_frame_relative_to_NED_projected_in_body.w;
        *_P(ret,i) = bodies[i].initial_velocity_of_body_frame_relative_to_NED_projected_in_body.p;
        *_Q(ret,i) = bodies[i].initial_velocity_of_body_frame_relative_to_NED_projected_in_body.q;
        *_R(ret,i) = bodies[i].initial_velocity_of_body_frame_relative_to_NED_projected_in_body.r;
        const auto norm = sqrt((double)SQUARE(q.w())+(double)SQUARE(q.x())+(double)SQUARE(q.y())+(double)SQUARE(q.z()));
        *_QR(ret,i) = q.w()/norm;
        *_QI(ret,i) = q.x()/norm;
        *_QJ(ret,i) = q.y()/norm;
        *_QK(ret,i) = q.z()/norm;
    }
    return ret;
}

void update_body_states(const StateType& x, Body& body, const size_t i)
{
    body.u = *_U(x,i);
    body.v = *_V(x,i);
    body.w = *_W(x,i);
    body.p = *_P(x,i);
    body.q = *_Q(x,i);
    body.r = *_R(x,i);
}
