/*
 * WindMeanVelocityProfile.cpp
 *
 *  Created on: 7 janv. 2020
 *      Author: mcharlou2016
 */

#include <Eigen/Dense>
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

#include "WindMeanVelocityProfile.hpp"

WindMeanVelocityProfile::WindMeanVelocityProfile(const Input& input): velocity(input.velocity), direction(cos(input.direction),sin(input.direction),0.)
{}

WindMeanVelocityProfile::~WindMeanVelocityProfile(){}

Eigen::Vector3d WindMeanVelocityProfile::get_wind(const Eigen::Vector3d& position, const double) const
{
	return get_wind_velocity(position(2))*direction;
}

WindMeanVelocityProfile::Input WindMeanVelocityProfile::parse(const std::string& yaml_input)
{
	std::stringstream stream(yaml_input);
	YAML::Parser parser(stream);
	YAML::Node node;
	parser.GetNextDocument(node);
	Input ret;
	ssc::yaml_parser::parse_uv(node["velocity"], ret.velocity);
	ssc::yaml_parser::parse_uv(node["direction"], ret.direction);
	return ret;
}

WindMeanVelocityProfile::Input::Input(): velocity(), direction()
{}
