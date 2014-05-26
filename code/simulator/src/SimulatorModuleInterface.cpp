/*
 * GravityModule.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "SimulatorModuleInterface.hpp"
#include "DataSource.hpp"

SimulatorModuleInterface::SimulatorModuleInterface(DataSource* const data_source, const std::string& module_name, const std::string& body_) : DataSourceModule(data_source, module_name), body(body_)
{
}

SimulatorModuleInterface::SimulatorModuleInterface(const SimulatorModuleInterface& rhs, DataSource* const data_source) : DataSourceModule(rhs, data_source), body(rhs.body)
{
}

std::string SimulatorModuleInterface::customize(const std::string& name) const
{
    return name + "(" + body + ")";
}
