/*
 * GravityModule.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "WaveModule.hpp"
#include "Wrench.hpp"
#include "DataSource.hpp"
#include "hydrostatic.hpp"
#include "WaveModelInterface.hpp"
#include "PointMatrix.hpp"

WaveModule::WaveModule(DataSource* const data_source, const std::string& module_name, const std::string& body_) : SimulatorModuleInterface(data_source, module_name, body_)
{
}

WaveModule::WaveModule(const WaveModule& rhs, DataSource* const data_source) : SimulatorModuleInterface(rhs, data_source)
{
}

DataSourceModule* WaveModule::clone() const
{
	return new WaveModule(*this);
}

DataSourceModule* WaveModule::clone(DataSource* const data_source) const
{
	return new WaveModule(*this, data_source);
}

void WaveModule::update() const
{
    const TR1(shared_ptr)<WaveModelInterface> wave_model = ds->get<TR1(shared_ptr)<WaveModelInterface> >("wave model");
    const TR1(shared_ptr)<Kinematics> k = ds->get<TR1(shared_ptr)<Kinematics> >("kinematics");
    const std::vector<double> dz = ds->read_only() ?
                                                     std::vector<double>()
                                                   : wave_model->get_relative_wave_height(*ds->get<TR1(shared_ptr)<PointMatrix> >(body),k);
    ds->set<std::vector<double> >(customize("wave heights"), dz);
}
