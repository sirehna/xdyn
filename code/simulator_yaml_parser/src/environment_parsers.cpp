/*
 * environment_parsers.cpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#include "environment_parsers.hpp"
#include "yaml.h"
#include "parse_unit_value.hpp"

double parse_default_wave_model(const std::string& yaml)
{
    double ret = 0;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    parse_uv(node["constant sea elevation in NED frame"], ret);
    return ret;
}

YamlWaveModel parse_waves(const std::string& yaml)
{
    YamlWaveModel ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    return ret;
}
