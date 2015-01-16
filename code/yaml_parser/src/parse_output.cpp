/*
 * parse_output.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */

#include "yaml.h"

#include "parse_output.hpp"

void operator >> (const YAML::Node& node, YamlOutput& f);
void operator >> (const YAML::Node& node, YamlOutput& f)
{
    node["filename"] >> f.filename;
    node["format"]   >> f.format;
    node["data"]     >> f.data;
}

std::vector<YamlOutput> parse_output(const std::string yaml)
{
    std::vector<YamlOutput> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    try
    {
        node["output"]         >> ret;
    }
    catch(std::exception& ) // Nothing to do: 'output' section is not mandatory
    {
    }
    return ret;
}

