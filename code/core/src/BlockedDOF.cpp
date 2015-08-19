/*
 * BlockedDOF.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */

#include "BlockedDOF.hpp"
#include "SimulatorYamlParserException.hpp"
#include <ssc/yaml_parser.hpp>
#include "yaml.h"

BlockedDOF::YamlCSVDOF::YamlCSVDOF() :
    YamlDOF<std::string>(),
    filename()
{
}

BlockedDOF::Yaml::Yaml() : from_yaml(), from_csv()
{
}

void operator >> (const YAML::Node& node, BlockedDOF::BlockableState& g);
void operator >> (const YAML::Node& node, BlockedDOF::InterpolationType& g);
void operator >> (const YAML::Node& node, BlockedDOF::YamlCSVDOF& g);
void operator >> (const YAML::Node& node, BlockedDOF::YamlDOF<std::vector<double> >& g);

void operator >> (const YAML::Node& node, BlockedDOF::BlockableState& g)
{
    std::string t;
    node >> t;
    if      (t == "u") g = BlockedDOF::BlockableState::U;
    else if (t == "v") g = BlockedDOF::BlockableState::V;
    else if (t == "w") g = BlockedDOF::BlockableState::W;
    else if (t == "p") g = BlockedDOF::BlockableState::P;
    else if (t == "q") g = BlockedDOF::BlockableState::Q;
    else if (t == "r") g = BlockedDOF::BlockableState::R;
    else
    {
        std::stringstream ss;
        ss << "Unrecognized state: '" << t << "'. Has to be one of 'u', 'v', 'w', 'p', 'q' or 'r'.";
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
}

void operator >> (const YAML::Node& node, BlockedDOF::InterpolationType& g)
{
    std::string t;
    node >> t;
    if      (t == "piecewise constant") g = BlockedDOF::InterpolationType::PIECEWISE_CONSTANT;
    else if (t == "linear")             g = BlockedDOF::InterpolationType::LINEAR;
    else if (t == "spline")             g = BlockedDOF::InterpolationType::SPLINE;
    else
    {
        std::stringstream ss;
        ss << "Unrecognized interpolation type: '" << t << "'. Has to be one of 'piecewise constant', 'linear', 'spline'";
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
}

void operator >> (const YAML::Node& node, BlockedDOF::YamlCSVDOF& g)
{
    node["filename"]      >> g.filename;
    node["interpolation"] >> g.interpolation;
    node["state"]         >> g.state;
    node["t"]             >> g.t;
    node["value"]         >> g.value;
}
void operator >> (const YAML::Node& node, BlockedDOF::YamlDOF<std::vector<double> >& g)
{
    node["interpolation"] >> g.interpolation;
    node["state"]         >> g.state;
    node["t"]             >> g.t;
    node["value"]         >> g.value;
}

BlockedDOF::Yaml BlockedDOF::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    BlockedDOF::Yaml ret;

    if (node.FindValue("from CSV"))  node["from CSV"]  >> ret.from_csv;
    if (node.FindValue("from YAML")) node["from YAML"] >> ret.from_yaml;
    return ret;
}
