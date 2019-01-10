/*
 * check_input_yaml.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef EXTERNAL_DATA_STRUCTURES_PARSERS_HPP_
#define EXTERNAL_DATA_STRUCTURES_PARSERS_HPP_
#include "yaml.h"
#include "YamlSimulatorInput.hpp"

size_t try_to_parse_positive_integer(const YAML::Node& node, const std::string& key);
void operator >> (const YAML::Node& node, YamlRotation& g);
void operator >> (const YAML::Node& node, YamlBody& b);
void operator >> (const YAML::Node& node, YamlModel& m);
void operator >> (const YAML::Node& node, YamlPosition& m);
void operator >> (const YAML::Node& node, YamlAngle& a);
void operator >> (const YAML::Node& node, YamlCoordinates& c);
void operator >> (const YAML::Node& node, YamlSpeed& s);
void operator >> (const YAML::Node& node, YamlDynamics& d);
void operator >> (const YAML::Node& node, YamlPoint& p);
void parse_point_with_name(const YAML::Node& node, YamlPoint& p, const std::string& name);
void parse_YamlDynamics6x6Matrix(const YAML::Node& node, YamlDynamics6x6Matrix& m, const bool parse_frame, const std::string& frame_name="");
void operator >> (const YAML::Node& node, YamlEnvironmentalConstants& f);
void operator >> (const YAML::Node& node, YamlBlockedDOF& b);
YamlBlockedDOF parse(const std::string& yaml);

template <typename T> void try_to_parse(const YAML::Node& node, const std::string& key, T& value)
{
    const YAML::Node * n = node.FindValue(key);
    if (n) (*n) >> value;
}
#endif /* EXTERNAL_DATA_STRUCTURES_PARSERS_HPP_ */
