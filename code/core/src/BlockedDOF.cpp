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
#include <ssc/csv_file_reader.hpp>
#include <ssc/text_file_reader.hpp>
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

BlockedDOF::Builder::Builder(const Yaml& yaml) : input(yaml)
{
}

BlockedDOF::Interpolator BlockedDOF::Builder::build(const BlockedDOF::YamlDOF<std::vector<double> >& y) const
{
    BlockedDOF::Interpolator ret;
    try
    {
        ret = build(y.t, y.value, y.interpolation);
    }
    catch(const ssc::exception_handling::Exception& e)
    {
        THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from YAML': " << e.get_message());
    }
    catch(const std::exception& e)
    {
        THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from YAML': " << e.what());
    }
    return ret;
}

BlockedDOF::Interpolator BlockedDOF::Builder::build(const BlockedDOF::YamlCSVDOF& y) const
{
    try
    {
        const ssc::text_file_reader::TextFileReader txt(y.filename);
        const ssc::csv_file_reader::CSVFileReader reader(txt.get_contents());
        auto m = reader.get_map();
        const auto it1 = m.find(y.t);
        if (it1 == m.end())
        {
            THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Unable to find column " << y.t << " in CSV file " << y.filename);
        }
        const auto it2 = m.find(y.value);
        if (it2 == m.end())
        {
            THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Unable to find column " << y.value << " in CSV file " << y.filename);
        }
        const auto t = it1->second;
        const auto state = it2->second;
        return build(t, state, y.interpolation);
    }
    catch(const std::exception& e)
    {
        THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.what());
    }
    return BlockedDOF::Interpolator();
}

std::map<BlockedDOF::BlockableState, BlockedDOF::Interpolator> BlockedDOF::Builder::get_forced_states() const
{
    std::map<BlockableState, Interpolator> ret;
    for (const auto y:input.from_csv) ret[y.state] = build(y);
    for (const auto y:input.from_yaml) ret[y.state] = build(y);
    return ret;
}

BlockedDOF::Interpolator BlockedDOF::Builder::build(const std::vector<double>& t, const std::vector<double>& state, const BlockedDOF::InterpolationType& type_of_interpolation) const
{
    switch(type_of_interpolation)
    {
        case InterpolationType::LINEAR:
            return BlockedDOF::Interpolator(new ssc::interpolation::LinearInterpolationVariableStep(t, state));
            break;
        case InterpolationType::PIECEWISE_CONSTANT:
            return BlockedDOF::Interpolator(new ssc::interpolation::PiecewiseConstantVariableStep<double>(t, state));
            break;
        case InterpolationType::SPLINE:
            return BlockedDOF::Interpolator(new ssc::interpolation::SplineVariableStep(t, state));
            break;
        default:
            break;
    }
    return Interpolator();
}
