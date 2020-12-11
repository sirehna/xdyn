/*
 * LogWindVelocityProfile.hpp
 *
 *  Created on: 11 déc. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_INC_LOGWINDVELOCITYPROFILE_HPP_
#define ENVIRONMENT_MODELS_INC_LOGWINDVELOCITYPROFILE_HPP_

#include "WindMeanVelocityProfile.hpp"

class LogWindVelocityProfile : public WindMeanVelocityProfile
{
public:
	struct Input : public WindMeanVelocityProfile::Input
	{
		Input();
		Input(const WindMeanVelocityProfile::Input& parent);
		double z0;
		double z_ref;
	};

	LogWindVelocityProfile(const Input& input);
	virtual ~LogWindVelocityProfile();

	virtual double get_wind_velocity(const double z) const override;
	static std::string model_name();
	static Input parse(const std::string& yaml_input);

private:
	double z0;
	double z_ref;
};

#endif /* ENVIRONMENT_MODELS_INC_LOGWINDVELOCITYPROFILE_HPP_ */
