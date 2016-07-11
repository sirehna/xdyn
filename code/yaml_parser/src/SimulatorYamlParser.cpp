/*
 * SimulatorYamlParser.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "SimulatorYamlParser.hpp"
#include "yaml.h"
#include "InvalidInputException.hpp"
#include <ssc/yaml_parser.hpp>
#include "external_data_structures_parsers.hpp"
#include "parse_commands.hpp"

SimulatorYamlParser::SimulatorYamlParser(const std::string& data) : YamlParser(data)
{
}

#define PARSE_OPTIONAL_KEY(key, dest) \
if(const YAML::Node *parameter = node.FindValue(key))\
    {\
        *parameter >> dest;\
    }

YamlSimulatorInput SimulatorYamlParser::parse() const
{
    YAML::Node node;
    convert_stream_to_yaml_node(contents, node);
    if (node.size() == 0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Something is wrong with the YAML data: no YAML nodes were detected by the YAML parser.");
    }
    YamlSimulatorInput ret;
    PARSE_OPTIONAL_KEY("environmental constants", ret.environmental_constants);
    PARSE_OPTIONAL_KEY("bodies", ret.bodies);
    node["rotations convention"] >> ret.rotations;
    PARSE_OPTIONAL_KEY("environment models", ret.environment);
    PARSE_OPTIONAL_KEY("commands", ret.commands);
    return ret;
}
