/*
 * SimObserver.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "SimObserver.hpp"
#include "StateMacros.hpp"
#include "Sim.hpp"
#include "YamlSimulatorInput.hpp"

SimObserver::SimObserver() : res(std::vector<Res>())
{
}

void SimObserver::observe(const Sim& s, const double t)
{
    Res r;
    r.t = t;
    r.x = s.state;
    res.push_back(r);
}

std::vector<Res> SimObserver::get() const
{
    return res;
}
