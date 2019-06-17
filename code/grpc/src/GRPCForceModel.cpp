/*
 * GRPCForceModel.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */


#include "GRPCForceModel.hpp"
#include "Body.hpp"
#include "yaml.h"
#include <ssc/macros.hpp>
#include <ssc/yaml_parser.hpp>

#include <ssc/kinematics.hpp>

std::string GRPCForceModel::model_name() {return "grpc force";}

GRPCForceModel::Input::Input() :
        url()
{
}

GRPCForceModel::Input GRPCForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    GRPCForceModel::Input ret;
    node["url"] >> ret.url;
    return ret;
}

GRPCForceModel::GRPCForceModel(const GRPCForceModel::Input& , const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ForceModel(model_name(), body_name_),
        env(env_)

{
}

ssc::kinematics::Wrench GRPCForceModel::operator()(const BodyStates& states, const double ) const
{
    return ssc::kinematics::Wrench(states.G);
}
