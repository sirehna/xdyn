/*
 * WindMeanVelocityProfile.hpp
 *
 *  Created on: 7 janv. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_INC_WINDMEANVELOCITYPROFILE_HPP_
#define ENVIRONMENT_MODELS_INC_WINDMEANVELOCITYPROFILE_HPP_

#include <Eigen/Dense>
#include <boost/optional/optional.hpp>
#include <memory>
#include "yaml-cpp/exceptions.h"

#include "InvalidInputException.hpp"
#include "YamlModel.hpp"
#include "WindModel.hpp"

class WindMeanVelocityProfile;
typedef std::shared_ptr<WindMeanVelocityProfile> WindMeanVelocityProfilePtr;
typedef std::function<boost::optional<WindMeanVelocityProfilePtr>(const YamlModel& yaml)> WindMeanVelocityProfileParser;

class WindMeanVelocityProfile : public WindModel
{
public:
	struct Input{
		Input();
		double velocity;
		double direction;
	};

	WindMeanVelocityProfile(const Input& input);
	virtual ~WindMeanVelocityProfile();

	Eigen::Vector3d get_wind(const Eigen::Vector3d& position, const double t) const override;
	virtual Eigen::Vector3d get_mean_wind(const double z) const=0;
	static Input parse(const std::string& yaml_input);

protected:
	double velocity;
	Eigen::Vector3d direction;
};

#endif /* ENVIRONMENT_MODELS_INC_WINDMEANVELOCITYPROFILE_HPP_ */
