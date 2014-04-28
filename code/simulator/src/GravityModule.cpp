/*
 * GravityModule.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "GravityModule.hpp"
#include "Wrench.hpp"
#include "DataSource.hpp"

GravityModule::GravityModule(DataSource* const data_source, const std::string& module_name, const std::string& body_) : DataSourceModule(data_source, module_name), body(body_)
{
}

GravityModule::GravityModule(const GravityModule& rhs, DataSource* const data_source) : DataSourceModule(rhs, data_source), body(rhs.body)
{
}

DataSourceModule* GravityModule::clone() const
{
	return new GravityModule(*this);
}

DataSourceModule* GravityModule::clone(DataSource* const data_source) const
{
	return new GravityModule(*this, data_source);
}

void GravityModule::update() const
{
}
