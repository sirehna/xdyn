/*
 * Sim.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>
#include <ssc/numeric.hpp>

#include "Observer.hpp"
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
             const ssc::data_source::DataSource& command_listener_) :
                 bodies(bodies_), forces(), controlled_forces(), env(env_),
                 _dx_dt(StateType(x.size(),0)), command_listener(command_listener_), sum_of_forces()
        {
            size_t i = 0;
            for (auto body:bodies)
            {
                forces[body->get_name()] = forces_.at(i);
                controlled_forces[body->get_name()] = controlled_forces_.at(i++);
            }
        }

        void feed_sum_of_forces(Observer& observer, const std::string& body_name)
        {
            observer.write(sum_of_forces[body_name].X(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",body_name,"Fx"},std::string("Fx(sum of forces,")+body_name+","+body_name+")"));
            observer.write(sum_of_forces[body_name].Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",body_name,"Fy"},std::string("Fy(sum of forces,")+body_name+","+body_name+")"));
            observer.write(sum_of_forces[body_name].Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",body_name,"Fz"},std::string("Fz(sum of forces,")+body_name+","+body_name+")"));
            observer.write(sum_of_forces[body_name].K(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",body_name,"Mx"},std::string("Mx(sum of forces,")+body_name+","+body_name+")"));
            observer.write(sum_of_forces[body_name].M(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",body_name,"My"},std::string("My(sum of forces,")+body_name+","+body_name+")"));
            observer.write(sum_of_forces[body_name].N(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",body_name,"Mz"},std::string("Mz(sum of forces,")+body_name+","+body_name+")"));
        }

        std::vector<BodyPtr> bodies;
        std::map<std::string,std::vector<ForcePtr> > forces;
        std::map<std::string,std::vector<ControllableForcePtr> > controlled_forces;
        EnvironmentAndFrames env;
        StateType _dx_dt;
        ssc::data_source::DataSource command_listener;
        std::map<std::string,ssc::kinematics::UnsafeWrench> sum_of_forces;
};

std::map<std::string,std::vector<ForcePtr> > Sim::get_forces() const
{
    return pimpl->forces;
}

std::vector<BodyPtr> Sim::get_bodies() const
{
    return pimpl->bodies;
}

EnvironmentAndFrames Sim::get_env() const
{
	return pimpl->env;
}

Sim::Sim(const std::vector<BodyPtr>& bodies,
         const std::vector<ListOfForces>& forces,
         const std::vector<ListOfControlledForces>& controlled_forces,
         const EnvironmentAndFrames& env,
         const StateType& x,
         const ssc::data_source::DataSource& command_listener) : state(x), pimpl(new Impl(bodies, forces, controlled_forces, env, x, command_listener))
{
    for (auto controlled_forces:pimpl->controlled_forces)
    {
        for (auto force:controlled_forces.second)
        {
            force->add_reference_frame(pimpl->env.k, pimpl->env.rot);
        }
    }
}

StateType Sim::normalize_quaternions(const StateType& all_states
                                    ) const
{
    StateType normalized = all_states;
    for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
    {
        const auto norm = sqrt((double)SQUARE(*_QR(normalized,i))+(double)SQUARE(*_QI(normalized,i))
                              +(double)SQUARE(*_QJ(normalized,i))+(double)SQUARE(*_QK(normalized,i)));
        if (not almost_equal(norm,1.0))
        {
            *_QR(normalized,i) /= norm;
            *_QI(normalized,i) /= norm;
            *_QJ(normalized,i) /= norm;
            *_QK(normalized,i) /= norm;
        }
    }
    return normalized;
}

void Sim::operator()(const StateType& x, StateType& dx_dt, double t)
{
    auto x_with_normalized_quaternions = normalize_quaternions(x);
    for (auto body: pimpl->bodies)
    {
        body->update(pimpl->env,x_with_normalized_quaternions,t);
        const auto Fext = sum_of_forces(x_with_normalized_quaternions, body, t);
        body->calculate_state_derivatives(Fext, x_with_normalized_quaternions, dx_dt, pimpl->env);
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

ssc::kinematics::UnsafeWrench Sim::sum_of_forces(const StateType& x, const BodyPtr& body, const double t)
{
    const Eigen::Vector3d uvw_in_body_frame = body->get_uvw_in_body_frame(x);
    const Eigen::Vector3d pqr = body->get_pqr(x);
    const auto states = body->get_states();
    pimpl->sum_of_forces[body->get_name()] = ssc::kinematics::UnsafeWrench(coriolis_and_centripetal(states.G,states.solid_body_inertia.get(),uvw_in_body_frame, pqr));
    const auto forces = pimpl->forces[body->get_name()];
    for (auto force:forces)
    {
        force->update(states, t);
        const ssc::kinematics::Wrench tau = force->get_force_in_body_frame();
        if (tau.get_frame() != body->get_name())
        {
            const ssc::kinematics::Transform T = pimpl->env.k->get(tau.get_frame(), body->get_name());
            const auto t = tau.change_frame_but_keep_ref_point(T);
            const ssc::kinematics::UnsafeWrench tau_body(states.G, t.force, t.torque + (t.get_point()-states.G).cross(t.force));
            pimpl->sum_of_forces[body->get_name()] += tau_body;
        }
        else
        {
            pimpl->sum_of_forces[body->get_name()] += tau;
        }
    }
    const auto controlled_forces = pimpl->controlled_forces[body->get_name()];
    for (auto force:controlled_forces)
    {
        const ssc::kinematics::Wrench tau = (*force)(states, t, pimpl->command_listener);
        const ssc::kinematics::Transform T = pimpl->env.k->get(tau.get_frame(), body->get_name());
        const auto t = tau.change_frame_but_keep_ref_point(T);
        const ssc::kinematics::UnsafeWrench tau_body(states.G, t.force, t.torque + (t.get_point()-states.G).cross(t.force));
        pimpl->sum_of_forces[body->get_name()] += tau_body;
    }
    return pimpl->sum_of_forces[body->get_name()];
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

void Sim::output(const StateType& x, Observer& obs, const double t) const
{
    for (auto forces:pimpl->forces)
    {
        for (auto force:forces.second) force->feed(obs);
    }
    for (auto body:pimpl->bodies) body->feed(x, obs, pimpl->env.rot);
    pimpl->env.feed(obs, t, pimpl->bodies, state);
    for (auto body:pimpl->bodies) pimpl->feed_sum_of_forces(obs, body->get_name());
}
