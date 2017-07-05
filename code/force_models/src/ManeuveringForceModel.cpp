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
#include "InvalidInputException.hpp"


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
    try
    {
        node["reference frame"] >> ret.frame_of_reference;
    }
    catch (YAML::Exception& e)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to parse 'reference frame': it should contain the sub-nodes 'frame', 'x', 'y', 'z', 'phi', 'theta' and 'psi'.");
    }
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
            try
            {
                std::string value;
                node[key] >> value;
                ret.var2expr[key] = value;
            }
            catch (const YAML::Exception&)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to parse key '" << key << "': in maneuvering model, all keys should be symbolic expressions (parsed as strings) appart from 'reference frame' and 'commands'.");
            }
        }
    }
    return ret;
}

ManeuveringForceModel::ManeuveringForceModel(const Yaml& data, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(data.name, data.commands, data.frame_of_reference, body_name_, env_),
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
