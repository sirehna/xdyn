/*
 * SimpleTrackKeepingController.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "yaml.h"

#include "BodyStates.hpp"
#include "external_data_structures_parsers.hpp"
#include "parse_unit_value.hpp"
#include "SimpleTrackKeepingController.hpp"

const std::string SimpleTrackKeepingController::model_name = "simple heading controller";

SimpleTrackKeepingController::Yaml::Yaml() : name(), ksi(0), Tp(0)
{
}

SimpleTrackKeepingController::Yaml SimpleTrackKeepingController::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    node["name"] >> ret.name;
    node["ksi"] >> ret.ksi;
    parse_uv(node["Tp"], ret.Tp);
    return ret;
}

SimpleTrackKeepingController::SimpleTrackKeepingController(const Yaml& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(input.name, {"psi_co"},
        YamlPosition(), body_name_, env_),
        ksi(input.ksi),
        omega0(2*PI/input.Tp),
        rotation_convention("angle", {"z","y'","x''"})
{
}

ssc::kinematics::Vector6d SimpleTrackKeepingController::get_force(const BodyStates& states, const double , std::map<std::string,double> commands) const
{
    ssc::kinematics::Vector6d ret = ssc::kinematics::Vector6d::Zero();

    const auto angles = states.get_angles(rotation_convention);
    const double delta_psi = commands["psi_co"] - angles.psi;
    const double sigma_zz = states.total_inertia->operator()(2,2);
    const double K_psi = sigma_zz*omega0*omega0;
    const double K_r = 2*ksi*omega0*sigma_zz;
    ret(5) = K_psi*delta_psi - K_r*states.r;
    return ret;
}
