/*
 * SimObserver.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "SimObserver.hpp"
#include "Sim.hpp"

SimObserver::SimObserver() : res(std::vector<Res>())
{
}

void SimObserver::observe(const Sim& s, const double t)
{
    res.push_back(Res(t,s.state));
}

std::vector<Res> SimObserver::get() const
{
    return res;
}
