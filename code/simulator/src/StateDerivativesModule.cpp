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
#include "coriolis_and_centripetal.hpp"

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
    const Matrix6x6 M = ds->get<Matrix6x6>(customize("total inertia"));
    const Matrix6x6 Mrb = ds->get<Matrix6x6>(customize("solid body inertia"));
    const Matrix6x6 Minv = ds->get<Matrix6x6>(customize("inverse of the total inertia"));
    const Point P = ds->get<Point>(customize("G"));
    const Eigen::Vector3d uvw(ds->get<double>(customize("u")),
                              ds->get<double>(customize("v")),
                              ds->get<double>(customize("w")));
    const Eigen::Vector3d pqr(ds->get<double>(customize("p")),
                              ds->get<double>(customize("q")),
                              ds->get<double>(customize("r")));
    std::vector<Wrench> f(1, coriolis_and_centripetal(P,Mrb,uvw, pqr));
    ds->set<Wrench>(customize("coriolis and centripetal forces"), f.front());
    if (not(ds->read_only()))
    {
        std::vector<std::string>::const_iterator it = forces.begin();
        for (;it != forces.end() ; ++it) f.push_back(ds->get<Wrench>(customize(*it)));
    }
    if (not(ds->read_only())) sum = sum::pairwise(f);
    ds->set(customize("sum of forces"), sum);
}
