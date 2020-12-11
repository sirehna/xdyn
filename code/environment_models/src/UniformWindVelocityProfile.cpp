/*
 * UniformWindVelocityProfile.cpp
 *
 *  Created on: 4 déc. 2020
 *      Author: mcharlou2016
 */

#include <math.h>

#include "UniformWindVelocityProfile.hpp"

UniformWindVelocityProfile::UniformWindVelocityProfile(const Input& input): WindMeanVelocityProfile(input)
{}

UniformWindVelocityProfile::~UniformWindVelocityProfile()
{}

std::string UniformWindVelocityProfile::model_name(){return "uniform wind";}

WindMeanVelocityProfile::Input UniformWindVelocityProfile::parse(const std::string& yaml_input)
{
	return WindMeanVelocityProfile::parse(yaml_input);
}

double UniformWindVelocityProfile::get_wind_velocity(const double) const
{
	return velocity;
}
