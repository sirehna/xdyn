/*
 * parse_commands.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "parse_commands.hpp"
#include <ssc/yaml_parser.hpp>
#include "yaml.h"

void operator >> (const YAML::Node& node, YamlCommands& c);
void operator >> (const YAML::Node& node, YamlCommands& c)
{
    node["name"] >> c.name;
    node["t"] >> c.t;
    for(YAML::Iterator it=node.begin();it!=node.end();++it)
    {
        std::string key = "";
        it.first() >> key;
        if ((key != "name") and (key != "t"))
        {
            std::vector<double> values;
            ssc::yaml_parser::parse_uv(node[key], values);
            c.commands[key] = values;
        }
    }
}

std::vector<YamlCommands> parse_command_yaml(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::vector<YamlCommands> ret;
    node >> ret;
    return ret;
}
