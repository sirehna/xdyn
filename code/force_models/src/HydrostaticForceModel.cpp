/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#include <Eigen/Dense>

#include <ssc/kinematics.hpp>

#include "Body.hpp"
#include "HydrostaticForceModel.hpp"
#include "Observer.hpp"
#include "QuadraticDampingForceModel.hpp"

const std::string HydrostaticForceModel::model_name = "hydrostatic";

HydrostaticForceModel::HydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : ForceModel(model_name, body_name_),
env(env_), centre_of_buoyancy(new Eigen::Vector3d())
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
    auto Tned2body = env.k->get(body, "NED");
    Tned2body.swap();
    auto TG2body = env.k->get(states.G.get_frame(), body);

    auto C = states.intersector->center_of_mass_immersed();

    if (C.all_facets_are_in_same_plane) C.volume = 0;

    for (size_t i = 0 ; i < 3 ; ++i) centre_of_buoyancy->operator()(i) = C.G(i);

    // Coordinates of the center of buoyancy in the BODY frame
    const ssc::kinematics::Point B(body, C.G);

    ssc::kinematics::Vector6d w;

    w << 0,
         0,
         -env.rho*env.g*C.volume,
         0,
         0,
         0;

    // The coordinates of the center of buoyancy (in the NED frame) are given by Tned2body.inverse()*B
    ssc::kinematics::Wrench ret(Tned2body.inverse()*B,w);
    const auto G = TG2body*states.G;
    ret = ret.change_frame_but_keep_ref_point(Tned2body);
    ssc::kinematics::Wrench ret2(B, ret.force, ret.torque);
    ret2 = ret2.change_point_of_application(G);
    ssc::kinematics::Wrench ret3(states.G, ret2.force, ret2.torque);
    return ret3;
}

ssc::kinematics::Point HydrostaticForceModel::get_centre_of_buoyancy() const
{
    return ssc::kinematics::Point(get_body_name(), centre_of_buoyancy->operator()(0),
                                                   centre_of_buoyancy->operator()(1),
                                                   centre_of_buoyancy->operator()(2));
}

void HydrostaticForceModel::extra_observations(Observer& observer) const
{
    observer.write(centre_of_buoyancy->operator()(0),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"Bx"},std::string("Bx")));
    observer.write(centre_of_buoyancy->operator()(1),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"By"},std::string("By")));
    observer.write(centre_of_buoyancy->operator()(2),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"Bz"},std::string("Bz")));
}
