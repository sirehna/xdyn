/*
 * update_kinematics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */
#include "update_kinematics.hpp"
#include "Body.hpp"
#include "BodyBuilder.hpp"
#include "StateMacros.hpp"
#include "yaml2eigen.hpp"
#include "YamlAngle.hpp"
#include "YamlBody.hpp"

#include <ssc/kinematics.hpp>

#define SQUARE(x) ((x)*(x))

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
