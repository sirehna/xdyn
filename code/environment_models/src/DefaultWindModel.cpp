/*
 * DefaultWindModel.cpp
 *
 *  Created on: 23 nov. 2020
 *      Author: mcharlou2016
 */

#include "DefaultWindModel.hpp"

DefaultWindModel::DefaultWindModel(int){}

DefaultWindModel::~DefaultWindModel(){}

std::string DefaultWindModel::model_name()
{
	return "no wind";
}

int DefaultWindModel::parse(const std::string&){return 0;}

Eigen::Vector3d DefaultWindModel::get_wind(const Eigen::Vector3d&, const double) const
{
	return Eigen::Vector3d::Zero();
}
