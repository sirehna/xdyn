/*
 * ConstantForce.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: cady
 */


#include "ConstantForceModel.hpp"

#include "Body.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

#include <ssc/kinematics.hpp>

std::string ConstantForceModel::model_name() {return "constant force";}

ConstantForceModel::Input::Input() :
        frame(),
        x(),
        y(),
        z(),
        X(),
        Y(),
        Z(),
        K(),
        M(),
        N()
{
}

ConstantForceModel::Input ConstantForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ConstantForceModel::Input ret;
    node["frame"] >> ret.frame;
    ssc::yaml_parser::parse_uv(node["x"], ret.x);
    ssc::yaml_parser::parse_uv(node["y"], ret.y);
    ssc::yaml_parser::parse_uv(node["z"], ret.z);
    ssc::yaml_parser::parse_uv(node["X"], ret.X);
    ssc::yaml_parser::parse_uv(node["Y"], ret.Y);
    ssc::yaml_parser::parse_uv(node["Z"], ret.Z);
    ssc::yaml_parser::parse_uv(node["K"], ret.K);
    return ret;
}

ConstantForceModel::ConstantForceModel(const ConstantForceModel::Input& , const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ForceModel(model_name(), body_name_),
        env(env_)
{
}

ssc::kinematics::Wrench ConstantForceModel::operator()(const BodyStates& , const double ) const
{
    return ssc::kinematics::Wrench();
}

