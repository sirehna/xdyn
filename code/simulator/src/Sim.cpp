/*
 * Sim.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>
#include "Sim.hpp"
#include "update_kinematics.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlWaveModelInput.hpp"
#include "SimException.hpp"

#define SQUARE(x) ((x)*(x))

Sim::Sim(const std::vector<Body>& bodies_,
         const std::vector<ListOfForces>& forces_,
         const std::vector<ListOfControlledForces>& controlled_forces_,
         const EnvironmentAndFrames& env_,
         const StateType& x,
         const ssc::data_source::DataSource& command_listener_,
         const bool there_are_surface_forces_) :
         state(x), bodies(bodies_), forces(forces_), controlled_forces(controlled_forces_), env(env_),
         _dx_dt(StateType(x.size(),0)), command_listener(command_listener_), there_are_surface_forces(there_are_surface_forces_)
{
    for (size_t i = 0 ; i < controlled_forces.size() ; ++i)
    {
        for (auto that_force = controlled_forces[i].begin() ; that_force != controlled_forces[i].end() ; ++that_force)
        {
            (*that_force)->add_reference_frame(env.k, env.rot);
        }
    }
}

void Sim::normalize_quaternions(StateType& all_states, //!< States of all bodies in the system
                                   const size_t i         //!< Index of the body under consideration
                               )
{
    const auto norm = sqrt((double)SQUARE(*_QR(all_states,i))+(double)SQUARE(*_QI(all_states,i))
                          +(double)SQUARE(*_QJ(all_states,i))+(double)SQUARE(*_QK(all_states,i)));
    *_QR(all_states,i) /= norm;
    *_QI(all_states,i) /= norm;
    *_QJ(all_states,i) /= norm;
    *_QK(all_states,i) /= norm;
}

void Sim::update_body(Body& body, const size_t i, const StateType& x, const double t) const
{
    update_body_states(x, body, i);
    if (there_are_surface_forces) body.update_intersection_with_free_surface(env, t);
    update_projection_of_z_in_mesh_frame(body);
}

void Sim::update_projection_of_z_in_mesh_frame(Body& body         //!< Body we wish to update
                                              ) const
{
    const ssc::kinematics::Point g_in_NED("NED", 0, 0, env.g);
    const ssc::kinematics::RotationMatrix ned2mesh = env.k->get("NED", std::string("mesh(") + body.name + ")").get_rot();
    body.down_direction_in_mesh_frame = ned2mesh*g_in_NED.v;
}

void Sim::operator()(const StateType& x, StateType& dx_dt, double t)
{
    auto x_with_normalized_quaternions = x;
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        normalize_quaternions(x_with_normalized_quaternions, i);
        update_kinematics(x_with_normalized_quaternions, bodies[i], i, env.k);
        update_body(bodies[i], i, x_with_normalized_quaternions, t);
        calculate_state_derivatives(sum_of_forces(x_with_normalized_quaternions, i, t), bodies[i].inverse_of_the_total_inertia, x_with_normalized_quaternions, dx_dt, i);
    }
    state = x_with_normalized_quaternions;
    _dx_dt = dx_dt;
}

void Sim::update_discrete_states()
{
}

void Sim::update_continuous_states()
{
}

StateType Sim::get_state_derivatives() const
{
    return _dx_dt;
}

std::vector<std::string> Sim::get_names_of_bodies() const
{
    std::vector<std::string> ret;
    for (auto that_body=bodies.begin() ; that_body != bodies.end() ; ++that_body)
    {
        ret.push_back(that_body->name);
    }
    return ret;
}

ssc::kinematics::UnsafeWrench Sim::sum_of_forces(const StateType& x, const size_t body, const double t)
{
    const Eigen::Vector3d& uvw_in_body_frame = Eigen::Vector3d::Map(_U(x,body));
    const Eigen::Vector3d& pqr = Eigen::Vector3d::Map(_P(x,body));
    ssc::kinematics::UnsafeWrench S(coriolis_and_centripetal(bodies[body].G,bodies[body].solid_body_inertia.get(),uvw_in_body_frame, pqr));
    for (auto that_force=forces[body].begin() ; that_force != forces[body].end() ; ++that_force)
    {
        const ssc::kinematics::Wrench tau = (**that_force)(bodies[body], t);
        if (tau.get_frame() != bodies[body].name)
        {
            const ssc::kinematics::Transform T = env.k->get(tau.get_frame(), bodies[body].name);
            S += tau.change_ref_point_then_change_frame(T);
        }
        else
        {
            S += tau;
        }
    }
    for (auto that_force=controlled_forces[body].begin() ; that_force != controlled_forces[body].end() ; ++that_force)
    {
        const ssc::kinematics::Wrench tau = (**that_force)(bodies[body], t, command_listener);
        const ssc::kinematics::Transform T = env.k->get(tau.get_frame(), bodies[body].name);
        S += tau.change_ref_point_then_change_frame(T);
    }
    return S;
}

void Sim::calculate_state_derivatives(const ssc::kinematics::Wrench& sum_of_forces,
                                      const MatrixPtr& inverse_of_the_total_inertia,
                                      const StateType& x,
                                      StateType& dx_dt,
                                      const size_t i) const
{
    // du/dt, dv/dt, dw/dt, dp/dt, dq/dt, dr/dt
    Eigen::Map<Eigen::Matrix<double,6,1> > dXdt(_U(dx_dt,i));

    dXdt = inverse_of_the_total_inertia->operator*(sum_of_forces.to_vector());

    // dx/dt, dy/dt, dz/dt
    const ssc::kinematics::RotationMatrix& R = env.k->get("NED", bodies[i].name).get_rot();
    const Eigen::Map<const Eigen::Vector3d> uvw_in_body_frame(_U(x,i));
    const Eigen::Vector3d uvw_in_ned_frame(R*uvw_in_body_frame);
    *_X(dx_dt,i) = uvw_in_ned_frame(0);
    *_Y(dx_dt,i) = uvw_in_ned_frame(1);
    *_Z(dx_dt,i) = uvw_in_ned_frame(2);

    // dqr/dt, dqi/dt, dqj/dt, dqk/dt
    const Eigen::Quaternion<double> q1(*_QR(x,i),
                                       *_QI(x,i),
                                       *_QJ(x,i),
                                       *_QK(x,i));
    const Eigen::Quaternion<double> q2(0,*_P(x,i),*_Q(x,i),*_R(x,i));
    const Eigen::Quaternion<double>& dq_dt = q1*q2;
    *_QR(dx_dt,i) = 0.5*(double)dq_dt.w();
    *_QI(dx_dt,i) = 0.5*(double)dq_dt.x();
    *_QJ(dx_dt,i) = 0.5*(double)dq_dt.y();
    *_QK(dx_dt,i) = 0.5*(double)dq_dt.z();
}

std::vector<ssc::kinematics::Point> Sim::get_waves(const double t//!< Current instant
                                                  ) const
{
    try
    {
        if (env.w.get())
        {
            for (size_t i = 0 ; i < bodies.size() ; ++i)
            {
                update_kinematics(state, bodies[i], i, env.k);
            }
            return env.w->get_waves_on_mesh(env.k, t);
        }
    }
    catch (const ssc::kinematics::KinematicsException& e)
    {
        std::stringstream ss;
        ss << "Error when calculating waves on mesh: the output reference frame does not exist (caught the following exception: " << e.what() << ")";
        THROW(__PRETTY_FUNCTION__, SimException, ss.str());
    }
    return std::vector<ssc::kinematics::Point>();
}
