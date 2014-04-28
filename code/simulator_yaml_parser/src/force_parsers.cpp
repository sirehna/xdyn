/*
 * forces_parsers.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */


#include "force_parsers.hpp"
#include "yaml.h"
#include "parse_unit_value.hpp"

GravityParameters parse_gravity(const std::string& yaml)
{
    GravityParameters ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);

    parse_uv(node["g"], ret.g);

    return ret;
}
