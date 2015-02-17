/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include "HydrostaticForceModel.hpp"

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "QuadraticDampingForceModel.hpp"

const std::string HydrostaticForceModel::model_name = "hydrostatic";

HydrostaticForceModel::HydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : ForceModel(model_name, body_name_),
env(env_)
{
}

bool HydrostaticForceModel::is_a_surface_force_model() const
{
    return true;
}

ssc::kinematics::Wrench HydrostaticForceModel::operator()(const BodyStates& states, const double) const
{
    const auto body = states.name;
    const auto mesh = std::string("mesh(") + body + ")";
    //const auto Tbody2mesh = env.k->get(body, mesh);
    auto Tned2body = env.k->get(body, "NED");
    Tned2body.swap();
    auto TG2body = env.k->get(states.G.get_frame(), body);

    //auto Tmesh2ned = (Tbody2mesh*Tned2body).inverse();

    const auto C = states.intersector->center_of_mass(states.intersector->begin_immersed(),
                                                      states.intersector->end_immersed());

    const ssc::kinematics::Point B(body, C.G);

    COUT(C.G);
    COUT(C.volume);


    ssc::kinematics::Vector6d w;

    w << 0,
         0,
         -env.rho*env.g*C.volume,
         0,
         0,
         0;

    ssc::kinematics::Wrench ret(Tned2body.inverse()*B,w);
    const auto G = TG2body*states.G;
    ret = ret.change_frame_but_keep_ref_point(Tned2body);
    ssc::kinematics::Wrench ret2(B, ret.force, ret.torque);
    ret2.change_point_of_application(G);
    ssc::kinematics::Wrench ret3(states.G, ret.force, ret.torque);
    COUT(ret3);
    return ret3;
}

