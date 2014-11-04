/*
 * SimulatorYamlParser.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "SimulatorYamlParser.hpp"
#include "yaml.h"
#include "SimulatorYamlParserException.hpp"
#include "parse_unit_value.hpp"
#include "external_data_structures_parsers.hpp"

SimulatorYamlParser::SimulatorYamlParser(const std::string& data) : YamlParser(data)
{
}

#define PARSE_OPTIONAL_KEY(key, dest) \
if(const YAML::Node *parameter = node.FindValue(key))\
    {\
        *parameter >> dest;\
    }

void parse_outputs(const YAML::Node& node, YamlSimulatorInput& ret);
void parse_outputs(const YAML::Node& node, YamlSimulatorInput& ret)
{
    PARSE_OPTIONAL_KEY("positions"             , ret.position_output)
    PARSE_OPTIONAL_KEY("angles"                , ret.angles_output)
    PARSE_OPTIONAL_KEY("linear velocities"     , ret.linear_velocities_output)
    PARSE_OPTIONAL_KEY("angular velocities"    , ret.angular_velocities_output)
    PARSE_OPTIONAL_KEY("linear accelerations"  , ret.linear_accelerations_output)
    PARSE_OPTIONAL_KEY("angular accelerations" , ret.angular_accelerations_output)
    PARSE_OPTIONAL_KEY("forces and torques"    , ret.forces_and_torques_output)
}

YamlSimulatorInput SimulatorYamlParser::parse() const
{
    YAML::Node node;
    convert_stream_to_yaml_node(contents, node);
    if (node.size() == 0)
    {
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Something is wrong with the YAML data: no YAML nodes were detected by the YAML parser.");
    }
    YamlSimulatorInput ret;
    PARSE_OPTIONAL_KEY("environmental constants", ret.environmental_constants);
    PARSE_OPTIONAL_KEY("bodies", ret.bodies);
    node["rotations convention"] >> ret.rotations;
    PARSE_OPTIONAL_KEY("environment models", ret.environment);
    PARSE_OPTIONAL_KEY("points", ret.points)
    PARSE_OPTIONAL_KEY("blocked degrees of freedom body/NED->body", ret.blocked_degrees_of_freedom)

    if(const YAML::Node *parameter = node.FindValue("outputs"))
    {
        parse_outputs(*parameter, ret);
    }
    return ret;
}
