/*
 * update_kinematics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"
#include "Body.hpp"
#include "BodyBuilder.hpp"
#include "Kinematics.hpp"
#include "StateMacros.hpp"
#include "YamlAngle.hpp"
#include "YamlBody.hpp"

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

StateType get_initial_states(const YamlRotation& convention, const std::vector<YamlBody>& bodies)
{
    StateType ret(13*bodies.size(),0);
    BodyBuilder builder(convention);
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        const YamlAngle angle = bodies[i].initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle;
        const RotationMatrix R = builder.angle2matrix(angle);
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
        *_QR(ret,i) = q.w();
        *_QI(ret,i) = q.x();
        *_QJ(ret,i) = q.y();
        *_QK(ret,i) = q.z();
    }
    return ret;
}
