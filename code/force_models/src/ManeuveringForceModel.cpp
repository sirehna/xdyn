/*
 * ManeuveringForceModel.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */

#include "external_data_structures_parsers.hpp"
#include "ManeuveringForceModel.hpp"

#include "yaml.h"


ManeuveringForceModel::Yaml::Yaml() :
    point_of_application(),
    var2expr()
{
}

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
