/*
 * BlockedDOF.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */

#include <map>

#include "BlockedDOF.hpp"
#include "BlockedDOFException.hpp"
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
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Unrecognized state: '" << t << "'. Has to be one of 'u', 'v', 'w', 'p', 'q' or 'r'.");
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
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Unrecognized interpolation type: '" << t << "'. Has to be one of 'piecewise constant', 'linear', 'spline'");
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

std::ostream& operator<<(std::ostream& os, const BlockedDOF::BlockableState& s);
std::ostream& operator<<(std::ostream& os, const BlockedDOF::BlockableState& s)
{
    switch(s)
    {
        case BlockedDOF::BlockableState::U:
            os << "u";
            break;
        case BlockedDOF::BlockableState::V:
            os << "v";
            break;
        case BlockedDOF::BlockableState::W:
            os << "w";
            break;
        case BlockedDOF::BlockableState::P:
            os << "p";
            break;
        case BlockedDOF::BlockableState::Q:
            os << "q";
            break;
        case BlockedDOF::BlockableState::R:
            os << "r";
            break;
        default:
            break;
    }
    return os;
}

void throw_if_already_defined(const BlockedDOF::BlockableState& state, std::map<BlockedDOF::BlockableState, bool>& defined);
void throw_if_already_defined(const BlockedDOF::BlockableState& state, std::map<BlockedDOF::BlockableState, bool>& defined)
{
    if (defined[state])
    {
        THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Attempting to define state " << state << " twice in 'blocked dof' section of the YAML file.");
    }
    defined[state] = true;
}

void BlockedDOF::check_states_are_not_defined_twice(const Yaml& input) const
{
    std::map<BlockedDOF::BlockableState, bool> defined_in_yaml, defined_in_csv;
    for (const auto state : input.from_yaml)
    {
        throw_if_already_defined(state.state, defined_in_yaml);
    }
    for (const auto state : input.from_csv)
    {
        throw_if_already_defined(state.state, defined_in_yaml);
        throw_if_already_defined(state.state, defined_in_csv);
    }
}

BlockedDOF::BlockedDOF(const Yaml& input)
{
    check_states_are_not_defined_twice(input);
}
