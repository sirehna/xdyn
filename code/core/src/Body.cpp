/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "Observer.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlBody.hpp"

Body::Body(const size_t i) : states(), idx(i)
{
}

Body::Body(const BodyStates& s, const size_t i) : states(s), idx(i)
{
}

Body::~Body()
{
}

BodyStates Body::get_states() const
{
    return states;
}

#define SQUARE(x) ((x)*(x))

ssc::kinematics::Point Body::get_origin(const StateType& x) const
{
    return ssc::kinematics::Point("NED", *_X(x,idx),
                                         *_Y(x,idx),
                                         *_Z(x,idx));
}

ssc::kinematics::Point Body::get_position_of_body_relative_to_mesh() const
{
    return ssc::kinematics::Point(std::string("mesh(")+states.name+")",
                                  states.x_relative_to_mesh,
                                  states.y_relative_to_mesh,
                                  states.z_relative_to_mesh);
}

ssc::kinematics::Transform Body::get_transform_from_mesh_to() const
{
    return ssc::kinematics::Transform(get_position_of_body_relative_to_mesh(), states.mesh_to_body, states.name);
}

ssc::kinematics::Transform Body::get_transform_from_ned_to(const StateType& x) const
{
    return ssc::kinematics::Transform(get_origin(x), states.get_rot_from_ned_to(x, idx), states.name);
}

void Body::update_kinematics(StateType x, const KinematicsPtr& k) const
{
    k->add(get_transform_from_ned_to(x));
}

void Body::update_body_states(const StateType& x)
{
    states.x = *_X(x,idx);
    states.y = *_Y(x,idx);
    states.z = *_Z(x,idx);
    states.u = *_U(x,idx);
    states.v = *_V(x,idx);
    states.w = *_W(x,idx);
    states.p = *_P(x,idx);
    states.q = *_Q(x,idx);
    states.r = *_R(x,idx);
    states.qr = *_QR(x,idx);
    states.qi = *_QI(x,idx);
    states.qj = *_QJ(x,idx);
    states.qk = *_QK(x,idx);
}

void Body::update_projection_of_z_in_mesh_frame(const double g,
                                                const ssc::kinematics::KinematicsPtr& k)
{
    const ssc::kinematics::Point g_in_NED("NED", 0, 0, g);
    const ssc::kinematics::RotationMatrix ned2mesh = k->get("NED", std::string("mesh(") + states.name + ")").get_rot();
    states.down_direction_in_mesh_frame = ned2mesh*g_in_NED.v;
}

void Body::update(const EnvironmentAndFrames& env, const StateType& x, const double t)
{
    update_kinematics(x,env.k);
    update_body_states(x);
    update_intersection_with_free_surface(env, t);
    update_projection_of_z_in_mesh_frame(env.g, env.k);
}

void Body::calculate_state_derivatives(const ssc::kinematics::Wrench& sum_of_forces,
                                         const StateType& x,
                                         StateType& dx_dt,
                                         const EnvironmentAndFrames& env) const
{
    // du/dt, dv/dt, dw/dt, dp/dt, dq/dt, dr/dt
    Eigen::Map<Eigen::Matrix<double,6,1> > dXdt(_U(dx_dt,idx));

    dXdt = states.inverse_of_the_total_inertia->operator*(sum_of_forces.to_vector());

    // dx/dt, dy/dt, dz/dt
    const ssc::kinematics::RotationMatrix& R = env.k->get("NED", states.name).get_rot();
    const Eigen::Map<const Eigen::Vector3d> uvw_in_body_frame(_U(x,idx));
    const Eigen::Vector3d uvw_in_ned_frame(R*uvw_in_body_frame);
    *_X(dx_dt,idx) = uvw_in_ned_frame(0);
    *_Y(dx_dt,idx) = uvw_in_ned_frame(1);
    *_Z(dx_dt,idx) = uvw_in_ned_frame(2);

    // dqr/dt, dqi/dt, dqj/dt, dqk/dt
    const Eigen::Quaternion<double> q1(*_QR(x,idx),
                                       *_QI(x,idx),
                                       *_QJ(x,idx),
                                       *_QK(x,idx));
    const Eigen::Quaternion<double> q2(0,*_P(x,idx),*_Q(x,idx),*_R(x,idx));
    const Eigen::Quaternion<double>& dq_dt = q1*q2;
    *_QR(dx_dt,idx) = 0.5*(double)dq_dt.w();
    *_QI(dx_dt,idx) = 0.5*(double)dq_dt.x();
    *_QJ(dx_dt,idx) = 0.5*(double)dq_dt.y();
    *_QK(dx_dt,idx) = 0.5*(double)dq_dt.z();
}

Eigen::Vector3d Body::get_uvw_in_body_frame(const StateType& x) const
{
    return Eigen::Vector3d::Map(_U(x,idx));
}

Eigen::Vector3d Body::get_pqr(const StateType& x) const
{
    return Eigen::Vector3d::Map(_P(x,idx));
}

void Body::feed(const StateType& x, Observer& observer) const
{
    observer.write(*_X(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"X"},std::string("x(")+states.name+")"));
    observer.write(*_Y(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"Y"},std::string("y(")+states.name+")"));
    observer.write(*_Z(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"Z"},std::string("z(")+states.name+")"));
    observer.write(*_U(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"U"},std::string("u(")+states.name+")"));
    observer.write(*_V(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"V"},std::string("v(")+states.name+")"));
    observer.write(*_W(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"W"},std::string("w(")+states.name+")"));
    observer.write(*_P(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"P"},std::string("p(")+states.name+")"));
    observer.write(*_Q(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"Q"},std::string("q(")+states.name+")"));
    observer.write(*_R(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"R"},std::string("r(")+states.name+")"));
    observer.write(*_QR(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qr"},std::string("qr(")+states.name+")"));
    observer.write(*_QI(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qi"},std::string("qi(")+states.name+")"));
    observer.write(*_QJ(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qj"},std::string("qj(")+states.name+")"));
    observer.write(*_QK(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qk"},std::string("qk(")+states.name+")"));
}

std::string Body::get_name() const
{
    return states.name;
}

ssc::kinematics::RotationMatrix Body::get_rot_from_ned_to(const StateType& x) const
{
    return states.get_rot_from_ned_to(x,idx);
}

ssc::kinematics::EulerAngles Body::get_angles(const StateType& all_states, const YamlRotation& c) const
{
    return states.get_angles(all_states, idx, c);
}
