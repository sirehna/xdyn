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
    const double m = ds->get<double>(std::string("m(")+body+")");
    const double g = ds->get<double>("g");
    const Point G(body);
    const Eigen::Vector3d force(0,0,m*g);
    const Eigen::Vector3d torque(0,0,0);
    const Wrench F(G, force, torque);
    ds->set<Wrench>(std::string("gravity(")+body+")", F);
}
