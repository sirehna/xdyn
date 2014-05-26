/*
 * GravityModule.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "HydrostaticModule.hpp"
#include "Wrench.hpp"
#include "DataSource.hpp"
#include "hydrostatic.hpp"
#include "WaveModelInterface.hpp"

HydrostaticModule::HydrostaticModule(DataSource* const data_source, const std::string& module_name, const std::string& body) : SimulatorModuleInterface(data_source, module_name, body)
{
}

HydrostaticModule::HydrostaticModule(const HydrostaticModule& rhs, DataSource* const data_source) : SimulatorModuleInterface(rhs, data_source)
{
}

DataSourceModule* HydrostaticModule::clone() const
{
	return new HydrostaticModule(*this);
}

DataSourceModule* HydrostaticModule::clone(DataSource* const data_source) const
{
	return new HydrostaticModule(*this, data_source);
}

void HydrostaticModule::update() const
{
    const double g = ds->get<double>("g");
    const Point G = ds->get<Point>(customize("G"));
    const double rho =  ds->get<double>("rho");
    const std::tr1::shared_ptr<Mesh> mesh = ds->get<std::tr1::shared_ptr<Mesh> >(body);
    const std::vector<double> dz = ds->get<std::vector<double> >(customize("wave heights"));
    const Wrench F = ds->read_only() ? Wrench() : hydrostatic::force(*mesh, G, rho, g, dz);
    ds->set<Wrench>(customize("hydrostatic"), F);
}
