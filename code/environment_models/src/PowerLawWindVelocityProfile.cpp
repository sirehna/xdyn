/*
 * PowerLawWindVelocityProfile.cpp
 *
 *  Created on: 10 déc. 2020
 *      Author: mcharlou2016
 */

#include <cmath>
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

#include "InvalidInputException.hpp"

#include "PowerLawWindVelocityProfile.hpp"

PowerLawWindVelocityProfile::PowerLawWindVelocityProfile(const Input& input): WindMeanVelocityProfile(input), alpha(input.alpha), z_ref(input.z_ref)
{
	//if(z_ref<=0) THROW(__PRETTY_FUNCTION__, InvalidInputException, "the reference height must be strictly positive");
	// TODO: find out why the compile error "'__ss' was not declared in this scope" occurs during expansion of THROW macro
}

PowerLawWindVelocityProfile::~PowerLawWindVelocityProfile ()
{}

std::string PowerLawWindVelocityProfile::model_name(){return "power law wind profile";}

PowerLawWindVelocityProfile::Input PowerLawWindVelocityProfile::parse(const std::string& yaml_input)
{
	Input ret(WindMeanVelocityProfile::parse(yaml_input));
	std::stringstream stream(yaml_input);
	YAML::Parser parser(stream);
	YAML::Node node;
	parser.GetNextDocument(node);
	node["alpha"] >> ret.alpha;
	ssc::yaml_parser::parse_uv(node["reference height"], ret.z_ref);
	return ret;
}

double PowerLawWindVelocityProfile::get_wind_velocity(const double z) const
{
	if(z<=0) return velocity*std::pow(-z/z_ref,alpha); // -z because the NED frame has negative z above sea level
	else return 0.;
}

PowerLawWindVelocityProfile::Input::Input(const WindMeanVelocityProfile::Input& parent): WindMeanVelocityProfile::Input(parent), alpha(), z_ref()
{}

PowerLawWindVelocityProfile::Input::Input(): WindMeanVelocityProfile::Input(), alpha(), z_ref()
{}

