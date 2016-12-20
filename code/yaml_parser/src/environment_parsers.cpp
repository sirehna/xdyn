/*
 * environment_parsers.cpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#include <sstream>

#include "environment_parsers.hpp"
#include "external_data_structures_parsers.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>
#include "InvalidInputException.hpp"

void operator >> (const YAML::Node& node, YamlDiscretization& g);
void operator >> (const YAML::Node& node, YamlSpectra& g);
void operator >> (const YAML::Node& node, YamlWaveOutput& g);
void operator >> (const YAML::Node& node, YamlStretching& g);

void get_yaml(const YAML::Node& node, std::string& out);

YamlDefaultWaveModel parse_default_wave_model(const std::string& yaml)
{
    YamlDefaultWaveModel ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ssc::yaml_parser::parse_uv(node["constant sea elevation in NED frame"], ret.zwave);
    try
    {
        node["output"]         >> ret.output;
    }
    catch(std::exception& ) // Nothing to do: 'output' section is not mandatory
    {
    }
    return ret;
}

YamlWaveModel parse_waves(const std::string& yaml)
{
    YamlWaveModel ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);

    try
    {
        node["discretization"] >> ret.discretization;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing section wave/discretization: " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    try
    {
        node["spectra"]        >> ret.spectra;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing section wave/spectra: " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    if (node.FindValue("output"))
    {
        try
        {
            node["output"]         >> ret.output;
        }
        catch(std::exception& e)
        {
            std::stringstream ss;
            ss << "Error parsing section wave/output: " << e.what();
            THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
        }
    }
    return ret;
}

void operator >> (const YAML::Node& node, YamlDiscretization& g)
{
    g.n = try_to_parse_positive_integer(node, "n");
    ssc::yaml_parser::parse_uv(node["omega min"], g.omega_min);
    ssc::yaml_parser::parse_uv(node["omega max"], g.omega_max);
    node["energy fraction"] >> g.energy_fraction;
}

void operator >> (const YAML::Node& node, YamlStretching& g)
{
    try
    {
        ssc::yaml_parser::parse_uv(node["h"], g.h);
        node["delta"] >> g.delta;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing wave stretching parameters ('wave/spectra/stretching' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
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
    node["stretching"] >> g.stretching;

    ssc::yaml_parser::parse_uv(node["depth"], g.depth);
}

void operator >> (const YAML::Node& node, YamlWaveOutput& g)
{
    node["frame of reference"] >> g.frame_of_reference;
    ssc::yaml_parser::parse_uv(node["mesh"]["xmin"], g.xmin);
    ssc::yaml_parser::parse_uv(node["mesh"]["xmax"], g.xmax);
    g.nx = try_to_parse_positive_integer(node["mesh"],"nx");
    ssc::yaml_parser::parse_uv(node["mesh"]["ymin"], g.ymin);
    ssc::yaml_parser::parse_uv(node["mesh"]["ymax"], g.ymax);
    g.ny = try_to_parse_positive_integer(node["mesh"],"ny");
}


YamlDiracDirection   parse_wave_dirac_direction(const std::string& yaml)
{
    YamlDiracDirection ret;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ssc::yaml_parser::parse_uv(node["waves propagating to"], ret.psi0);
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing Dirac directional spreading parameters ('environment models/model/spectra/directional spreading' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
return ret;
}

YamlDiracSpectrum    parse_wave_dirac_spectrum(const std::string& yaml)
{
    YamlDiracSpectrum ret;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ssc::yaml_parser::parse_uv(node["Hs"], ret.Hs);
        ssc::yaml_parser::parse_uv(node["omega0"], ret.omega0);
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing Dirac spectrum parameters ('environment models/model/spectra/directional spreading' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    return ret;
}

YamlJonswap          parse_jonswap(const std::string& yaml)
{
    YamlJonswap ret;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ssc::yaml_parser::parse_uv(node["Hs"], ret.Hs);
        ssc::yaml_parser::parse_uv(node["Tp"], ret.Tp);
        node["gamma"] >> ret.gamma;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing JONSWAP wave spectrum parameters ('wave' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    return ret;
}

YamlPiersonMoskowitz parse_pierson_moskowitz(const std::string& yaml)
{
    YamlPiersonMoskowitz ret;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ssc::yaml_parser::parse_uv(node["Hs"], ret.Hs);
        ssc::yaml_parser::parse_uv(node["Tp"], ret.Tp);
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing Pierson-Moskowitz spectrum parameters ('wave' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    return ret;
}

YamlBretschneider    parse_bretschneider(const std::string& yaml)
{
    YamlBretschneider ret;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ssc::yaml_parser::parse_uv(node["Hs"], ret.Hs);
        ssc::yaml_parser::parse_uv(node["Tp"], ret.Tp);
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing Bretschneider spectrum parameters ('wave' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    return ret;
}

YamlCos2s            parse_cos2s(const std::string& yaml)
{
    YamlCos2s ret;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ssc::yaml_parser::parse_uv(node["waves propagating to"], ret.psi0);
        node["s"] >> ret.s;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing cos2s directional spreading parameters ('wave' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    return ret;
}

int                  parse_airy(const std::string& yaml)
{
    int ret = 0;
    try
    {
        std::stringstream stream(yaml);
        YAML::Parser parser(stream);
        YAML::Node node;
        parser.GetNextDocument(node);
        ret = (int)try_to_parse_positive_integer(node, "seed of the random data generator");
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing Airy wave model parameters ('wave' section in the YAML file): " << e.what();
        THROW(__PRETTY_FUNCTION__, InvalidInputException, ss.str());
    }
    return ret;
}

