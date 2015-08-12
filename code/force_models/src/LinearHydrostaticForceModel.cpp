/*
 * LinearHydrostaticForceModel.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "LinearHydrostaticForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Body.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

#include <ssc/kinematics.hpp>

const std::string LinearHydrostaticForceModel::model_name = "linear hydrostatics";

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

LinearHydrostaticForceModel::LinearHydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env) : ForceModel(model_name, body_name_)
{}

ssc::kinematics::Wrench LinearHydrostaticForceModel::operator()(const BodyStates& states, const double) const
{
    return ssc::kinematics::Wrench(states.G,
                                   Eigen::Vector3d(0,0,0),
                                   Eigen::Vector3d(0,0,0));
}
