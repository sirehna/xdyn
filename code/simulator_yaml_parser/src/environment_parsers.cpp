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
void operator >> (const YAML::Node& node, YamlSpectra& g);
void get_yaml(const YAML::Node& node, std::string& out);

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
    node["spectra"]        >> ret.spectra;
    return ret;
}

void operator >> (const YAML::Node& node, YamlDiscretization& g)
{
    node["n"] >> g.n;
    parse_uv(node["omega min"], g.omega_min);
    parse_uv(node["omega max"], g.omega_max);
}


void get_yaml(const YAML::Node& node, std::string& out)
{
    YAML::Emitter e;
    e << node;
    out = e.c_str();
}

void operator >> (const YAML::Node& node, YamlSpectra& g)
{
    node["model"] >> g.model;
    get_yaml(node, g.model_yaml);

    node["directional spreading"]["type"] >> g.directional_spreading_type;
    get_yaml(node["directional spreading"], g.directional_spreading_yaml);

    node["spectral density"]["type"] >> g.spectral_density_type;
    get_yaml(node["spectral density"], g.spectral_density_yaml);
}
