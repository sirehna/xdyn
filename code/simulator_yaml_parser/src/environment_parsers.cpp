/*
 * environment_parsers.cpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#include "environment_parsers.hpp"
#include "yaml.h"
#include "parse_unit_value.hpp"

void operator >> (const YAML::Node& node, YamlDiscretization& g);

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

    node["discretization"] >> ret.discretization;
    return ret;
}

void operator >> (const YAML::Node& node, YamlDiscretization& g)
{
    node["n"] >> g.n;
    parse_uv(node["omega min"], g.omega_min);
    parse_uv(node["omega max"], g.omega_max);
}

