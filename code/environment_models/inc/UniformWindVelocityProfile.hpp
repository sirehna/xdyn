/*
 * UniformWindVelocityProfile.hpp
 *
 *  Created on: 4 déc. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_INC_UNIFORMWINDVELOCITYPROFILE_HPP_
#define ENVIRONMENT_MODELS_INC_UNIFORMWINDVELOCITYPROFILE_HPP_

#include <string>
#include <Eigen/Dense>

#include "WindMeanVelocityProfile.hpp"

class UniformWindVelocityProfile : public WindMeanVelocityProfile
{
public:
	UniformWindVelocityProfile(const Input& input);
	virtual ~UniformWindVelocityProfile();

	virtual double get_wind_velocity(const double z) const override;
	static std::string model_name();
	static Input parse(const std::string& yaml_input);
};

#endif /* ENVIRONMENT_MODELS_INC_UNIFORMWINDVELOCITYPROFILE_HPP_ */
