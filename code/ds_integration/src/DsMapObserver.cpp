/*
 * DsMapObserver.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: cady
 */

#include <algorithm> // std::transform

#include "DsMapObserver.hpp"
#include "DsSystem.hpp"

DsMapObserver::DsMapObserver() : DsObserver(), v(std::vector<std::map<std::string,double> >())
{
}

void DsMapObserver::observe(const DSSystem& sys, const double t)
{
    const std::vector<double> x = sys.state;
    const std::vector<std::string> state_names = sys.get_state_names();
    std::map<std::string,double> res;
    res["t"] = t;
    for (size_t i = 0 ; i < x.size() ; ++i)
    {
        res[state_names.at(i)] = x.at(i);
    }
    v.push_back(res);
}


std::vector<std::map<std::string,double> > DsMapObserver::get() const
{
    return v;
}
