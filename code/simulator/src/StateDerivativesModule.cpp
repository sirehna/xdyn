/*
 * StateDerivativesModule.cpp
 *
 *  Created on: May 29, 2014
 *      Author: cady
 */

#include "StateDerivativesModule.hpp"
#include "Wrench.hpp"
#include "DataSource.hpp"
#include "pairwise_sum.hpp"

StateDerivativesModule::StateDerivativesModule(DataSource* const data_source, const std::string& body, const std::vector<std::string>& forces_) : SimulatorModuleInterface(data_source, "sum of forces", body), forces(forces_)
{

}

StateDerivativesModule::StateDerivativesModule(const StateDerivativesModule& rhs, DataSource* const data_source) : SimulatorModuleInterface(rhs, data_source), forces(rhs.forces)
{

}

DataSourceModule* StateDerivativesModule::clone() const
{
    return new StateDerivativesModule(*this);
}

DataSourceModule* StateDerivativesModule::clone(DataSource* const data_source) const
{
    return new StateDerivativesModule(*this, data_source);
}

void StateDerivativesModule::update() const
{
    Wrench sum;
    std::vector<Wrench> f;
    if (not(ds->read_only()))
    {
        std::vector<std::string>::const_iterator it = forces.begin();
        for (;it != forces.end() ; ++it) f.push_back(ds->get<Wrench>(customize(*it)));
    }
    if (not(ds->read_only())) sum = sum::pairwise(f);
    ds->set(customize("sum of forces"), sum);
}
