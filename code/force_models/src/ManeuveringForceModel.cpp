/*
 * ManeuveringForceModel.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */

#include "external_data_structures_parsers.hpp"
#include "ManeuveringForceModel.hpp"
#include "maneuvering_compiler.hpp"
#include "maneuvering_DataSource_builder.hpp"
#include "yaml.h"


ManeuveringForceModel::Yaml::Yaml() :
    point_of_application(),
    var2expr()
{
}

const std::string ManeuveringForceModel::model_name = "maneuvering";

ManeuveringForceModel::Yaml ManeuveringForceModel::parse(const std::string& yaml)
{
    ManeuveringForceModel::Yaml ret;
    std::stringstream stream(yaml);
    std::stringstream ss;
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    node["point of application (in body frame)"] >> ret.point_of_application;
    for(YAML::Iterator it=node.begin();it!=node.end();++it)
    {
        std::string key = "";
        it.first() >> key;
        if (key != "point of application (in body frame)")
        {
            std::string value;
            node[key] >> value;
            ret.var2expr[key] = value;
        }
    }
    return ret;
}

ManeuveringForceModel::ManeuveringForceModel(const Yaml& data, const std::string& body_name_, const EnvironmentAndFrames&) :
        ForceModel(model_name, body_name_),
        point_of_application(body_name_, data.point_of_application.x, data.point_of_application.y, data.point_of_application.z),
        m()
{
    for (auto var2expr:data.var2expr) m[var2expr.first] = maneuvering::compile(var2expr.second);
}

ssc::kinematics::Wrench ManeuveringForceModel::operator()(const BodyStates& states, const double t) const
{
    auto ds = maneuvering::build_ds(m);
    ds.check_in(__PRETTY_FUNCTION__);
    ds.set("states", states);
    ds.set("t", t);
    ssc::kinematics::Vector6d tau = ssc::kinematics::Vector6d::Zero();
    tau(0) = ds.get<double>("X");
    tau(1) = ds.get<double>("Y");
    tau(2) = ds.get<double>("Z");
    tau(3) = ds.get<double>("K");
    tau(4) = ds.get<double>("M");
    tau(5) = ds.get<double>("N");
    ds.check_out();
    return ssc::kinematics::Wrench(point_of_application, tau);
}
