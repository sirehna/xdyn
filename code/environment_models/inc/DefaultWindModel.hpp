/*
 * DefaultWindModel.hpp
 *
 *  Created on: 23 nov. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_INC_DEFAULTWINDMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_DEFAULTWINDMODEL_HPP_

#include <string>

#include "WindModel.hpp"

class DefaultWindModel : public WindModel
{
public:
	DefaultWindModel(int);// Constructor argument is a dummy in order to be able to call DefaultWindModel(DefaultWindModel::parse(...)) from parser
	virtual ~DefaultWindModel();

	virtual Eigen::Vector3d get_wind(const Eigen::Vector3d& position, const double t) const override;

	static std::string model_name();
	static int parse(const std::string&);
};

#endif /* ENVIRONMENT_MODELS_INC_DEFAULTWINDMODEL_HPP_ */
