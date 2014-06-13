/*
 * DsSystem.cpp
 *
 *  Created on: 25 mars 2014
 *      Author: cec
 */

#include "DsSystem.hpp"
#include "DataSource.hpp"

DSSystem::DSSystem(DataSource& ds_) : ds(ds_), state(ds.get_states())
{
}

void DSSystem::operator()(const std::vector<double>& x, std::vector<double>& dx_dt, double t)
{
    ds.check_in(__PRETTY_FUNCTION__);
    ds.set<double>("t", t);
    ds.set_states(x);
    ds.get_derivatives(dx_dt);
    ds.check_out();
}

void DSSystem::update_discrete_states()
{

}

void DSSystem::update_continuous_states()
{
    ds.check_in(__PRETTY_FUNCTION__);
    ds.set_states(state);
    ds.check_out();
}

std::vector<std::string> DSSystem::get_state_names() const
{
    return ds.get_state_names();
}
