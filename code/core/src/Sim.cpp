/*
 * Sim.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */


#include "Observer.hpp"
#include "Sim.hpp"
#include "update_kinematics.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlWaveModelInput.hpp"
#include "InternalErrorException.hpp"

#include <ssc/kinematics.hpp>
#include <ssc/numeric.hpp>

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
                 bodies(bodies_), name2bodyptr(), forces(), controlled_forces(), env(env_),
                 _dx_dt(StateType(x.size(),0)), command_listener(command_listener_), sum_of_forces_in_body_frame(),
                 sum_of_forces_in_NED_frame()
        {
            size_t i = 0;
            for (auto body:bodies)
            {
                forces[body->get_name()] = forces_.at(i);
                controlled_forces[body->get_name()] = controlled_forces_.at(i++);
                name2bodyptr[body->get_name()] = body;
            }
        }

        void feed_sum_of_forces(Observer& observer, const std::string& body_name)
        {
            feed_sum_of_forces(observer, sum_of_forces_in_body_frame[body_name], body_name, body_name);
            feed_sum_of_forces(observer, sum_of_forces_in_NED_frame[body_name], body_name, "NED");
        }

        void feed_sum_of_forces(Observer& observer, ssc::kinematics::UnsafeWrench& W, const std::string& body_name, const std::string& frame)
        {
            observer.write(W.X(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",frame,"Fx"},std::string("Fx(sum of forces,")+body_name+","+frame+")"));
            observer.write(W.Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",frame,"Fy"},std::string("Fy(sum of forces,")+body_name+","+frame+")"));
            observer.write(W.Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",frame,"Fz"},std::string("Fz(sum of forces,")+body_name+","+frame+")"));
            observer.write(W.K(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",frame,"Mx"},std::string("Mx(sum of forces,")+body_name+","+frame+")"));
            observer.write(W.M(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",frame,"My"},std::string("My(sum of forces,")+body_name+","+frame+")"));
            observer.write(W.N(),DataAddressing(std::vector<std::string>{"efforts",body_name,"sum of forces",frame,"Mz"},std::string("Mz(sum of forces,")+body_name+","+frame+")"));
        }

        std::vector<BodyPtr> bodies;
        std::map<std::string,BodyPtr> name2bodyptr;
        std::map<std::string,std::vector<ForcePtr> > forces;
        std::map<std::string,std::vector<ControllableForcePtr> > controlled_forces;
        EnvironmentAndFrames env;
        StateType _dx_dt;
        ssc::data_source::DataSource command_listener;
        std::map<std::string,ssc::kinematics::UnsafeWrench> sum_of_forces_in_body_frame;
        std::map<std::string,ssc::kinematics::UnsafeWrench> sum_of_forces_in_NED_frame;
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
}

StateType Sim::normalize_quaternions(const StateType& all_states
                                    ) const
{
    StateType normalized = all_states;
    for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
    {
        const auto norm = std::hypot(std::hypot(std::hypot(*_QR(normalized,i),*_QI(normalized,i)),*_QJ(normalized,i)),*_QK(normalized,i));
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

void Sim::operator()(const StateType& x, StateType& dxdt, double t)
{
    dx_dt(x, dxdt, t);
    state = normalize_quaternions(x);
    pimpl->_dx_dt = dxdt;
}

void Sim::force_states(StateType& x, const double t) const
{
    for (auto body: pimpl->bodies)
    {
        body->force_states(x, t);
    }
}

void Sim::dx_dt(const StateType& x, StateType& dxdt, const double t)
{
    for (auto body: pimpl->bodies)
    {
        body->update(pimpl->env,x,t);
        const auto Fext = sum_of_forces(x, body, t);
        body->calculate_state_derivatives(Fext, x, dxdt, t, pimpl->env);
    }
}

void Sim::update_discrete_states()
{
}

void Sim::update_continuous_states()
{
}

ssc::kinematics::UnsafeWrench Sim::sum_of_forces(const StateType& x, const BodyPtr& body, const double t)
{
    const Eigen::Vector3d uvw = body->get_uvw(x);
    const Eigen::Vector3d pqr = body->get_pqr(x);
    const auto states = body->get_states();
    pimpl->sum_of_forces_in_body_frame[body->get_name()] = ssc::kinematics::UnsafeWrench(coriolis_and_centripetal(states.G,states.solid_body_inertia.get(),uvw, pqr));
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
            pimpl->sum_of_forces_in_body_frame[body->get_name()] += tau_body;
        }
        else
        {
            pimpl->sum_of_forces_in_body_frame[body->get_name()] += tau;
        }
    }
    const auto controlled_forces = pimpl->controlled_forces[body->get_name()];
    for (auto force:controlled_forces)
    {
        const ssc::kinematics::Wrench tau = force->operator()(states, t, pimpl->command_listener, pimpl->env.k, states.G);
        pimpl->sum_of_forces_in_body_frame[body->get_name()] += tau;
    }
    pimpl->sum_of_forces_in_NED_frame[body->get_name()] = ForceModel::project_into_NED_frame(pimpl->sum_of_forces_in_body_frame[body->get_name()],states.get_rot_from_ned_to_body());
    return pimpl->sum_of_forces_in_body_frame[body->get_name()];
}

ssc::kinematics::PointMatrix Sim::get_waves(const double t//!< Current instant
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
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating waves on mesh: the output reference frame does not exist (" << e.get_message() << ")");
    }
    return ssc::kinematics::PointMatrix("NED",0);
}

void Sim::output(const StateType& x, Observer& obs, const double t) const
{
    StateType x_with_forced_states;
    for (auto body: pimpl->bodies)
    {
        x_with_forced_states = body->block_states_if_necessary(x,t);
    }
    const auto normalized_x = normalize_quaternions(x_with_forced_states);
    for (auto forces:pimpl->forces)
    {
        for (auto force:forces.second) force->feed(obs);
    }
    for (auto controlled_forces:pimpl->controlled_forces)
    {
        for (auto force:controlled_forces.second)
        {
            const auto body_name = controlled_forces.first;
            const auto body = pimpl->name2bodyptr[body_name];
            const auto G = body->get_origin(x);
            force->feed(obs,pimpl->env.k,G);
        }
    }
    for (auto body:pimpl->bodies)
    {
        body->feed(normalized_x, obs, pimpl->env.rot);
        auto dF = body->get_delta_F(pimpl->_dx_dt,pimpl->sum_of_forces_in_body_frame[body->get_name()]);
        obs.write((double)dF(0),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Fx"},std::string("Fx(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(1),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Fy"},std::string("Fy(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(2),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Fz"},std::string("Fz(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(3),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Mx"},std::string("Mx(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(4),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"My"},std::string("My(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(5),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Mz"},std::string("Mz(blocked states,")+body->get_name()+","+body->get_name()+")"));
    }
    pimpl->env.feed(obs, t, pimpl->bodies, normalized_x);
    for (auto body:pimpl->bodies)
    {
        pimpl->feed_sum_of_forces(obs, body->get_name());
    }
}

void Sim::set_bodystates(const State& states_history)
{
    pimpl->bodies.at(0)->set_states_history(states_history);
    if (not(states_history.x.is_empty()))
    {
        // Initialize current body state with last state in history
        state = states_history.get_StateType(states_history.x.size()-1);
    }
}

void Sim::set_command_listener(const std::map<std::string, double>& new_commands)
{
    for(const auto c : new_commands)
    {
        pimpl->command_listener.set(c.first, c.second);
    }
}

void Sim::reset_history()
{
    for (auto body:pimpl->bodies)
    {
        body->reset_history();
    }
}
