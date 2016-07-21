/*
 * ManeuveringForceModel.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "external_data_structures_parsers.hpp"
#include "ManeuveringForceModel.hpp"
#include "maneuvering_compiler.hpp"
#include "maneuvering_DataSource_builder.hpp"
#include "yaml.h"
#include "yaml2eigen.hpp"


ManeuveringForceModel::Yaml::Yaml() :
    name(),
    frame_of_reference(),
    commands(),
    var2expr()
{
}

std::string ManeuveringForceModel::model_name() {return "maneuvering";}

ManeuveringForceModel::Yaml ManeuveringForceModel::parse(const std::string& yaml)
{
    ManeuveringForceModel::Yaml ret;
    std::stringstream stream(yaml);
    std::stringstream ss;
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    node["reference frame"] >> ret.frame_of_reference;
    node["name"] >> ret.name;
    for(YAML::Iterator it=node.begin();it!=node.end();++it)
    {
        std::string key = "";
        it.first() >> key;
        if (key == "commands")
        {
            node[key] >> ret.commands;
        }
        else if (key != "reference frame")
        {
            std::string value;
            node[key] >> value;
            ret.var2expr[key] = value;
        }

    }
    return ret;
}

ManeuveringForceModel::ManeuveringForceModel(const Yaml& data, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(data.name, data.commands, data.frame_of_reference, body_name_, env_),
        point_of_application(data.name, data.frame_of_reference.coordinates.x, data.frame_of_reference.coordinates.y, data.frame_of_reference.coordinates.z),
        m(),
        ds(new ssc::data_source::DataSource())
{
    env.k->add(make_transform(data.frame_of_reference, data.name, env.rot));
    for (auto var2expr:data.var2expr)
    {
        m[var2expr.first] = maneuvering::compile(var2expr.second, env.rot);
    }
    ds->set("g", env.g);
    ds->set("nu", env.nu);
    ds->set("rho", env.rho);
    maneuvering::build_ds(*ds, m);
}

ssc::kinematics::Vector6d ManeuveringForceModel::get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const
{
    ds->check_in(__PRETTY_FUNCTION__);
    ds->set("states", states);
    ds->set("t", t);
    for (const auto command:commands) ds->set(command.first, command.second);

    ssc::kinematics::Vector6d tau = ssc::kinematics::Vector6d::Zero();
    tau(0) = ds->get<double>("X");
    tau(1) = ds->get<double>("Y");
    tau(2) = ds->get<double>("Z");
    tau(3) = ds->get<double>("K");
    tau(4) = ds->get<double>("M");
    tau(5) = ds->get<double>("N");
    ds->check_out();
    return tau;
}
