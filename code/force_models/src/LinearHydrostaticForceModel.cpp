/*
 * LinearHydrostaticForceModel.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#include "LinearHydrostaticForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

#include <ssc/kinematics.hpp>

std::string LinearHydrostaticForceModel::model_name() {return "linear hydrostatics";}

LinearHydrostaticForceModel::Input::Input() :
            z_eq(),
            theta_eq(),
            phi_eq(),
            K1(),
            K2(),
            K3(),
            x1(),
            y1(),
            x2(),
            y2(),
            x3(),
            y3(),
            x4(),
            y4()
{
}

LinearHydrostaticForceModel::Input LinearHydrostaticForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    LinearHydrostaticForceModel::Input ret;
    ssc::yaml_parser::parse_uv(node["z eq"], ret.z_eq);
    ssc::yaml_parser::parse_uv(node["phi eq"], ret.phi_eq);
    ssc::yaml_parser::parse_uv(node["theta eq"], ret.theta_eq);
    node["K row 1"] >> ret.K1;
    node["K row 2"] >> ret.K2;
    node["K row 3"] >> ret.K3;
    ssc::yaml_parser::parse_uv(node["x1"], ret.x1);
    ssc::yaml_parser::parse_uv(node["x2"], ret.x2);
    ssc::yaml_parser::parse_uv(node["x3"], ret.x3);
    ssc::yaml_parser::parse_uv(node["x4"], ret.x4);
    ssc::yaml_parser::parse_uv(node["y1"], ret.y1);
    ssc::yaml_parser::parse_uv(node["y2"], ret.y2);
    ssc::yaml_parser::parse_uv(node["y3"], ret.y3);
    ssc::yaml_parser::parse_uv(node["y4"], ret.y4);
    return ret;
}

LinearHydrostaticForceModel::LinearHydrostaticForceModel(const Input& input, const std::string& body_name_, const EnvironmentAndFrames& env_) : ForceModel(model_name(), body_name_),
        K(),
        P1(body_name_, input.x1, input.y1, 0),
        P2(body_name_, input.x2, input.y2, 0),
        P3(body_name_, input.x3, input.y3, 0),
        P4(body_name_, input.x4, input.y4, 0),
        z_eq(input.z_eq),
        theta_eq(input.theta_eq),
        phi_eq(input.phi_eq),
        env(env_),
        d12((P1-P2).norm()),
        d34((P3-P4).norm()),
        d13((P1-P3).norm()),
        d24((P2-P4).norm())
{
    K(0,0) = input.K1.at(0);
    K(0,1) = input.K1.at(1);
    K(0,2) = input.K1.at(2);
    K(1,0) = input.K2.at(0);
    K(1,1) = input.K2.at(1);
    K(1,2) = input.K2.at(2);
    K(2,0) = input.K3.at(0);
    K(2,1) = input.K3.at(1);
    K(2,2) = input.K3.at(2);
}

double LinearHydrostaticForceModel::compute_zbar(const std::vector<double>& z) const
{
    return 0.25*(z[0]+z[1]+z[2]+z[3]);
}

double LinearHydrostaticForceModel::compute_phibar(const std::vector<double>& z) const
{
    return 0.5*(atan(z[1]-z[0])/d12 + atan(z[3]-z[2])/d34);
}

double LinearHydrostaticForceModel::compute_thetabar(const std::vector<double>& z) const
{
    return 0.5*(atan(z[1]-z[3])/d24 + atan(z[0]-z[2])/d13);
}

std::vector<double> LinearHydrostaticForceModel::get_zH(const double t) const
{
    std::vector<double> ret;
    std::vector<double> x(4);
    std::vector<double> y(4);
    auto T = env.k->get("NED", P1.get_frame());
    T.swap();
    auto P_ = T*P1;
    x.at(0) = P_.x(); y.at(0) = P_.y();
    P_ = T*P2;
    x.at(1) = P_.x(); y.at(1) = P_.y();
    P_ = T*P3;
    x.at(2) = P_.x(); y.at(2) = P_.y();
    P_ = T*P4;
    x.at(3) = P_.x(); y.at(3) = P_.y();

    ret = env.w->wave_height(x, y, t);
    return ret;
}

ssc::kinematics::Wrench LinearHydrostaticForceModel::operator()(const BodyStates& states, const double t) const
{
    const auto z = get_zH(t);
    const double zbar = compute_zbar(z);
    const double phibar = compute_phibar(z);
    const double thetabar = compute_thetabar(z);
    const auto angles = states.get_angles(env.rot);
    const Eigen::Vector3d v(states.z() - zbar - z_eq, angles.phi - phibar - phi_eq, angles.theta - thetabar - theta_eq);
    const Eigen::Vector3d F = -K*v;

    return ssc::kinematics::Wrench(states.G,
                                   Eigen::Vector3d(0,0,F(0)),
                                   Eigen::Vector3d(F(1),F(2),0));
}
