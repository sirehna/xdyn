/*
 * WindModel.hpp
 *
 *  Created on: 7 janv. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_INC_WINDMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_WINDMODEL_HPP_

#include <Eigen/Dense>
#include <memory>
#include <ssc/macros.hpp>
#include <boost/optional/optional.hpp>
#include "yaml-cpp/exceptions.h"

#include "YamlModel.hpp"
#include "InvalidInputException.hpp"

class WindModel;
typedef std::shared_ptr<WindModel> WindModelPtr;
typedef std::function<boost::optional<WindModelPtr>(const YamlModel&)> WindParser;

class WindModel
{
public:
	WindModel();
	virtual ~WindModel();
	virtual Eigen::Vector3d get_wind(const Eigen::Vector3d& position, const double t) const=0;

	template <typename WindType>
	static WindParser build_parser()
	{
		auto parser = [](const YamlModel& yaml) -> boost::optional<WindModelPtr>
		{
			boost::optional<WindModelPtr> ret;
			if (yaml.model == WindType::model_name())
			{
				std::string context = "Invalid input data for model '" + WindType::model_name() + "'.";
				try
				{
					ret.reset(WindModelPtr(new WindType(WindType::parse(yaml.yaml))));
				}
				catch (const InvalidInputException& exception)
				{
					THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << "The error was: " << exception.get_message() << std::endl << "Model containing error is defined line "
						  << yaml.index_of_first_line_in_global_yaml << " of the YAML file." << std::endl);
				}
				catch (const YAML::Exception& exception)
				{
					const size_t line_number = yaml.index_of_first_line_in_global_yaml;
					THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << "Model containing error is defined line "
						  << line_number << " of the YAML file." << std::endl << "The error was: " << exception.msg);
				}
			}
			return ret;
		};
		return parser;
	}
};

#endif /* ENVIRONMENT_MODELS_INC_WINDMODEL_HPP_ */
