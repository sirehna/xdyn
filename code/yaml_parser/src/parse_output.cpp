/*
 * parse_output.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */

#include "yaml.h"

#include "parse_output.hpp"

void operator >> (const YAML::Node& node, YamlOutput2& f);
void operator >> (const YAML::Node& node, YamlOutput2& f)
{
    node["filename"] >> f.filename;
    node["format"]   >> f.format;
    node["data"]     >> f.data;
}

std::vector<YamlOutput2> parse_output(const std::string yaml)
{
    std::vector<YamlOutput2> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    node["output"] >> ret;
    return ret;
}

