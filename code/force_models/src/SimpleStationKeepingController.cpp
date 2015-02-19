/*
 * SimpleStationKeepingController.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: cady
 */

#include "SimpleStationKeepingController.hpp"

#include "yaml.h"

#include "BodyStates.hpp"
#include "external_data_structures_parsers.hpp"
#include "parse_unit_value.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

const std::string SimpleStationKeepingController::model_name = "simple station-keeping controller";

SimpleStationKeepingController::Yaml::Yaml() :
            name(),
            ksi_x(),
            T_x(),
            ksi_y(),
            T_y(),
            ksi_psi(),
            T_psi()
{
}

SimpleStationKeepingController::Yaml SimpleStationKeepingController::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    node["name"] >> ret.name;
    node["ksi_x"] >> ret.ksi_x;
    parse_uv(node["T_x"], ret.T_x);
    node["ksi_y"] >> ret.ksi_y;
    parse_uv(node["T_y"], ret.T_y);
    node["ksi_psi"] >> ret.ksi_psi;
    parse_uv(node["T_psi"], ret.T_psi);
    return ret;
}

SimpleStationKeepingController::SimpleStationKeepingController(const Yaml& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(input.name, {"x_co", "y_co", "psi_co"}, YamlPosition(YamlCoordinates(),YamlAngle(), body_name_), body_name_, env_),
        ksi_x(input.ksi_x),
        omega_x(2*PI/input.T_x),
        ksi_y(input.ksi_y),
        omega_y(2*PI/input.T_y),
        ksi_psi(input.ksi_psi),
        omega_psi(2*PI/input.T_psi),
        rotation_convention("angle", {"z","y'","x''"})
{

}

ssc::kinematics::Vector6d SimpleStationKeepingController::get_force(const BodyStates& states, const double , std::map<std::string,double> commands) const
{
    ssc::kinematics::Vector6d ret = ssc::kinematics::Vector6d::Zero();

    const auto angles = states.get_angles(rotation_convention);
    const double delta_x = commands["x_co"] - states.x();
    const double delta_y = commands["y_co"] - states.y();
    const double delta_psi = commands["psi_co"] - angles.psi;
    const double sigma_xx = states.total_inertia->operator()(0,0);
    const double sigma_yy = states.total_inertia->operator()(1,1);
    const double sigma_psipsi = states.total_inertia->operator()(5,5);
    const double K_x = sigma_xx*omega_x*omega_x;
    const double K_u = 2*ksi_x*omega_x*sigma_xx;
    const double K_y = sigma_yy*omega_y*omega_y;
    const double K_v = 2*ksi_y*omega_y*sigma_yy;
    const double K_psi = sigma_psipsi*omega_psi*omega_psi;
    const double K_r = 2*ksi_psi*omega_psi*sigma_psipsi;
    ret(0) = K_x*delta_x - K_u*states.u();
    ret(1) = K_y*delta_y - K_v*states.v();
    ret(5) = K_psi*delta_psi - K_r*states.r();
    return ret;
}

