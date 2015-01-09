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

class Sim::Impl
{
    public:
        Impl(const std::vector<BodyPtr>& bodies_,
             const std::vector<ListOfForces>& forces_,
             const std::vector<ListOfControlledForces>& controlled_forces_,
             const EnvironmentAndFrames& env_,
             const StateType& x,
             const ssc::data_source::DataSource& command_listener_,
             const bool there_are_surface_forces_) :
                 bodies(bodies_), forces(forces_), controlled_forces(controlled_forces_), env(env_),
                 _dx_dt(StateType(x.size(),0)), command_listener(command_listener_), there_are_surface_forces(there_are_surface_forces_),
                 outputted_forces()
        {

        }

        std::vector<BodyPtr> bodies;
        std::vector<ListOfForces> forces;
        std::vector<ListOfControlledForces> controlled_forces;
        EnvironmentAndFrames env;
        StateType _dx_dt;
        ssc::data_source::DataSource command_listener;
        bool there_are_surface_forces;
        OuputtedForces outputted_forces;
};

Sim::Sim(const std::vector<BodyPtr>& bodies,
         const std::vector<ListOfForces>& forces,
         const std::vector<ListOfControlledForces>& controlled_forces,
         const EnvironmentAndFrames& env,
         const StateType& x,
         const ssc::data_source::DataSource& command_listener,
         const bool there_are_surface_forces) : state(x), pimpl(new Impl(bodies, forces, controlled_forces, env, x, command_listener, there_are_surface_forces))
{
    for (size_t i = 0 ; i < pimpl->controlled_forces.size() ; ++i)
    {
        for (auto that_force = pimpl->controlled_forces[i].begin() ; that_force != pimpl->controlled_forces[i].end() ; ++that_force)
        {
            (*that_force)->add_reference_frame(pimpl->env.k, pimpl->env.rot);
        }
    }
    fill_force_map_with_zeros(pimpl->outputted_forces);
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

void Sim::update_body(Body& body, const size_t , const StateType& x, const double t) const
{
    body.update_body_states(x);
    if (pimpl->there_are_surface_forces) body.update_intersection_with_free_surface(pimpl->env, t);
    body.update_projection_of_z_in_mesh_frame(pimpl->env.g, pimpl->env.k);
}

void Sim::operator()(const StateType& x, StateType& dx_dt, double t)
{
    auto x_with_normalized_quaternions = x;
    for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
    {
        normalize_quaternions(x_with_normalized_quaternions, i);
        pimpl->bodies[i]->update_kinematics(x_with_normalized_quaternions,pimpl->env.k);
        update_body(*(pimpl->bodies[i]), i, x_with_normalized_quaternions, t);
        calculate_state_derivatives(sum_of_forces(x_with_normalized_quaternions, i, t), pimpl->bodies[i]->states.inverse_of_the_total_inertia, x_with_normalized_quaternions, dx_dt, i);
    }
    state = x_with_normalized_quaternions;
    pimpl->_dx_dt = dx_dt;
}

void Sim::update_discrete_states()
{
}

void Sim::update_continuous_states()
{
}

StateType Sim::get_state_derivatives() const
{
    return pimpl->_dx_dt;
}

std::vector<std::string> Sim::get_names_of_bodies() const
{
    std::vector<std::string> ret;
    for (auto that_body=pimpl->bodies.begin() ; that_body != pimpl->bodies.end() ; ++that_body)
    {
        ret.push_back((*that_body)->states.name);
    }
    return ret;
}

OuputtedForces Sim::get_forces() const
{
    return pimpl->outputted_forces;
}

std::vector<double> Sim::get_forces_as_a_vector_of_doubles() const
{
    std::vector<double> ret;
    const auto m = pimpl->outputted_forces;
    for (auto it1 = m.begin() ; it1 != m.end() ; ++it1)
    {
        for (auto it2 = it1->second.begin() ; it2 != it1->second.end() ; ++it2)
        {
            const std::vector<double> mm(it2->second.data(),it2->second.data()+6);
            ret.insert(ret.end(), mm.begin(), mm.end());
        }
    }
    return ret;
}

void Sim::fill_force_map_with_zeros(OuputtedForces& m) const
{
    for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
    {
        for (auto that_force=pimpl->forces[i].begin() ; that_force != pimpl->forces[i].end() ; ++that_force)
        {
            fill_force(m, pimpl->bodies[i]->states.name, (*that_force)->get_name(), ssc::kinematics::Wrench(ssc::kinematics::Point(), ssc::kinematics::Vector6d::Zero()));
        }
        for (auto that_force=pimpl->controlled_forces[i].begin() ; that_force != pimpl->controlled_forces[i].end() ; ++that_force)
        {
            fill_force(m, pimpl->bodies[i]->states.name, (*that_force)->get_name(), ssc::kinematics::Wrench(ssc::kinematics::Point(), ssc::kinematics::Vector6d::Zero()));
        }
    }
}

std::vector<std::string> Sim::get_force_names() const
{
    std::vector<std::string> ret;
    OuputtedForces m;
    fill_force_map_with_zeros(m);
    for (auto it1 = m.begin() ; it1 != m.end() ; ++it1)
    {
        for (auto it2 = it1->second.begin() ; it2 != it1->second.end() ; ++it2)
        {
            ret.push_back(it2->first + " acting on " + it1->first);
        }
    }
    return ret;
}

VectorOfStringModelForEachBody Sim::get_vector_of_string_model_for_each_body() const
{
    VectorOfStringModelForEachBody ret;
    OuputtedForces m;
    fill_force_map_with_zeros(m);
    for (auto it1 = m.begin() ; it1 != m.end() ; ++it1)
    {
        std::vector<std::string> bodyModel;
        for (auto it2 = it1->second.begin() ; it2 != it1->second.end() ; ++it2)
        {
            bodyModel.push_back(it2->first);
        }
        ret.push_back(std::make_pair(it1->first,bodyModel));
    }
    return ret;
}

void Sim::fill_force(OuputtedForces& ret, const std::string& body_name, const std::string& force_name, const ssc::kinematics::Wrench& tau) const
{
    ssc::kinematics::Vector6d s;
    s<<tau.force,
       tau.torque;
    ret[body_name][force_name] = s;
}

ssc::kinematics::UnsafeWrench Sim::sum_of_forces(const StateType& x, const size_t body, const double t)
{
    const Eigen::Vector3d& uvw_in_body_frame = Eigen::Vector3d::Map(_U(x,body));
    const Eigen::Vector3d& pqr = Eigen::Vector3d::Map(_P(x,body));
    ssc::kinematics::UnsafeWrench S(coriolis_and_centripetal(pimpl->bodies[body]->states.G,pimpl->bodies[body]->states.solid_body_inertia.get(),uvw_in_body_frame, pqr));
    for (auto that_force=pimpl->forces[body].begin() ; that_force != pimpl->forces[body].end() ; ++that_force)
    {
        const ssc::kinematics::Wrench tau = (**that_force)(pimpl->bodies[body]->states, t);
        if (tau.get_frame() != pimpl->bodies[body]->states.name)
        {
            const ssc::kinematics::Transform T = pimpl->env.k->get(tau.get_frame(), pimpl->bodies[body]->states.name);
            const auto t = tau.change_frame_but_keep_ref_point(T);
            const ssc::kinematics::UnsafeWrench tau_body(pimpl->bodies[body]->states.G, t.force, t.torque + (t.get_point()-pimpl->bodies[body]->states.G).cross(t.force));
            fill_force(pimpl->outputted_forces,pimpl->bodies[body]->states.name, (*that_force)->get_name(), tau_body);
            S += tau_body;
        }
        else
        {
            fill_force(pimpl->outputted_forces,pimpl->bodies[body]->states.name, (*that_force)->get_name(), tau);
            S += tau;
        }
    }
    for (auto that_force=pimpl->controlled_forces[body].begin() ; that_force != pimpl->controlled_forces[body].end() ; ++that_force)
    {
        const ssc::kinematics::Wrench tau = (**that_force)(pimpl->bodies[body]->states, t, pimpl->command_listener);
        const ssc::kinematics::Transform T = pimpl->env.k->get(tau.get_frame(), pimpl->bodies[body]->states.name);
        const auto t = tau.change_frame_but_keep_ref_point(T);
        const ssc::kinematics::UnsafeWrench tau_body(pimpl->bodies[body]->states.G, t.force, t.torque + (t.get_point()-pimpl->bodies[body]->states.G).cross(t.force));
        fill_force(pimpl->outputted_forces,pimpl->bodies[body]->states.name, (*that_force)->get_name(), tau_body);
        S += tau_body;
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
    const ssc::kinematics::RotationMatrix& R = pimpl->env.k->get("NED", pimpl->bodies[i]->states.name).get_rot();
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
        if (pimpl->env.w.get())
        {
            for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
            {
                pimpl->bodies[i]->update_kinematics(state,pimpl->env.k);
            }
            return pimpl->env.w->get_waves_on_mesh(pimpl->env.k, t);
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
