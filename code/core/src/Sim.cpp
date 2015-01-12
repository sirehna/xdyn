/*
 * Sim.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>
#include <ssc/numeric.hpp>
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
                 _dx_dt(StateType(x.size(),0)), command_listener(command_listener_),
                 outputted_forces()
        {
            size_t i = 0;
            for (auto body:bodies)
            {
                forces[body->states.name] = forces_.at(i);
                controlled_forces[body->states.name] = controlled_forces_.at(i++);
            }
        }

        std::vector<BodyPtr> bodies;
        std::map<std::string,std::vector<ForcePtr> > forces;
        std::map<std::string,std::vector<ControllableForcePtr> > controlled_forces;
        EnvironmentAndFrames env;
        StateType _dx_dt;
        ssc::data_source::DataSource command_listener;
        OuputtedForces outputted_forces;
};

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
    fill_force_map_with_zeros(pimpl->outputted_forces);
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
    for (auto body:pimpl->bodies)
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

std::map<std::string,double> Sim::get_states(const StateType& x) const
{
    std::map<std::string,double> ret;
    size_t i = 0;
    for (auto body:pimpl->bodies)
    {
        ret[std::string("x(") + body->states.name + ")"] = *_X(x,i);
        ret[std::string("y(") + body->states.name + ")"] = *_Y(x,i);
        ret[std::string("z(") + body->states.name + ")"] = *_Z(x,i);
        ret[std::string("u(") + body->states.name + ")"] = *_U(x,i);
        ret[std::string("v(") + body->states.name + ")"] = *_V(x,i);
        ret[std::string("w(") + body->states.name + ")"] = *_W(x,i);
        ret[std::string("p(") + body->states.name + ")"] = *_P(x,i);
        ret[std::string("q(") + body->states.name + ")"] = *_Q(x,i);
        ret[std::string("r(") + body->states.name + ")"] = *_R(x,i);
        ret[std::string("qr(") + body->states.name + ")"] = *_QR(x,i);
        ret[std::string("qi(") + body->states.name + ")"] = *_QI(x,i);
        ret[std::string("qj(") + body->states.name + ")"] = *_QJ(x,i);
        ret[std::string("qk(") + body->states.name + ")"] = *_QK(x,i);
        i++;
    }
    return ret;
}

void Sim::fill_force_map_with_zeros(OuputtedForces& m) const
{
    for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
    {
        auto forces = pimpl->forces[pimpl->bodies[i]->states.name];
        auto controlled_forces = pimpl->controlled_forces[pimpl->bodies[i]->states.name];
        for (auto force:forces)
        {
            fill_force(m, pimpl->bodies[i]->states.name, force->get_name(), ssc::kinematics::Wrench(ssc::kinematics::Point(), ssc::kinematics::Vector6d::Zero()));
        }
        for (auto force:controlled_forces)
        {
            fill_force(m, pimpl->bodies[i]->states.name, force->get_name(), ssc::kinematics::Wrench(ssc::kinematics::Point(), ssc::kinematics::Vector6d::Zero()));
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

ssc::kinematics::UnsafeWrench Sim::sum_of_forces(const StateType& x, const BodyPtr& body, const double t)
{
    const Eigen::Vector3d uvw_in_body_frame = body->get_uvw_in_body_frame(x);
    const Eigen::Vector3d pqr = body->get_pqr(x);
    ssc::kinematics::UnsafeWrench S(coriolis_and_centripetal(body->states.G,body->states.solid_body_inertia.get(),uvw_in_body_frame, pqr));
    const auto forces = pimpl->forces[body->states.name];
    for (auto force:forces)
    {
        const ssc::kinematics::Wrench tau = (*force)(body->states, t);
        if (tau.get_frame() != body->states.name)
        {
            const ssc::kinematics::Transform T = pimpl->env.k->get(tau.get_frame(), body->states.name);
            const auto t = tau.change_frame_but_keep_ref_point(T);
            const ssc::kinematics::UnsafeWrench tau_body(body->states.G, t.force, t.torque + (t.get_point()-body->states.G).cross(t.force));
            fill_force(pimpl->outputted_forces,body->states.name, force->get_name(), tau_body);
            S += tau_body;
        }
        else
        {
            fill_force(pimpl->outputted_forces,body->states.name, force->get_name(), tau);
            S += tau;
        }
    }
    const auto controlled_forces = pimpl->controlled_forces[body->states.name];
    for (auto force:controlled_forces)
    {
        const ssc::kinematics::Wrench tau = (*force)(body->states, t, pimpl->command_listener);
        const ssc::kinematics::Transform T = pimpl->env.k->get(tau.get_frame(), body->states.name);
        const auto t = tau.change_frame_but_keep_ref_point(T);
        const ssc::kinematics::UnsafeWrench tau_body(body->states.G, t.force, t.torque + (t.get_point()-body->states.G).cross(t.force));
        fill_force(pimpl->outputted_forces,body->states.name, force->get_name(), tau_body);
        S += tau_body;
    }
    return S;
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

SurfaceElevationGrid Sim::get_waves_as_a_grid(
        const double t//!< Current instant
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
            return pimpl->env.w->get_waves_on_mesh_as_a_grid(pimpl->env.k, t);
        }
    }
    catch (const ssc::kinematics::KinematicsException& e)
    {
        std::stringstream ss;
        ss << "Error when calculating waves on mesh: the output reference frame does not exist (caught the following exception: " << e.what() << ")";
        THROW(__PRETTY_FUNCTION__, SimException, ss.str());
    }
    return SurfaceElevationGrid();
}

std::pair<std::size_t,std::size_t> Sim::get_waves_mesh_size() const
{
    if (pimpl->env.w.get())
    {
        return pimpl->env.w->get_output_mesh_size();
    }
    return std::make_pair((std::size_t)0,(std::size_t)0);
}
