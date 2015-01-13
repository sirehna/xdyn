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
void operator >> (const YAML::Node& node, YamlDynamics6x6Matrix& m);
void operator >> (const YAML::Node& node, YamlOutput& p);
void operator >> (const YAML::Node& node, YamlPositionOutput& p);
void operator >> (const YAML::Node& node, YamlAnglesOutput& p);
void operator >> (const YAML::Node& node, YamlForcesAndTorquesOutput& f);
void operator >> (const YAML::Node& node, YamlEnvironmentalConstants& f);

#endif /* EXTERNAL_DATA_STRUCTURES_PARSERS_HPP_ */
