/*
 * DsSystem.cpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#include "DsSystem.hpp"
#include "DataSource.hpp"

DSSystem::DSSystem(DataSource& ds_) : state(std::vector<double>()), ds(ds_)
{
    state = ds.get_states();
}

void DSSystem::operator()(const std::vector<double>& x, std::vector<double>& dx_dt, double )
{
    ds.set_states(x);
    ds.get_derivatives(dx_dt);
}

void DSSystem::update_discrete_states()
{

}

void DSSystem::update_continuous_states()
{
    ds.set_states(state);
}
