/*
 * Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "Observer.hpp"
#include "ObserverException.hpp"
#include "Sim.hpp"

Observer::Observer(const std::vector<std::string>& data_) : initialized(false), stuff_to_write(data_), serialize(), initialize()
{
    stuff_to_write.insert(stuff_to_write.begin(), "t");
}

void Observer::observe(const Sim& sys, const double t)
{
    write(t, std::vector<std::string>(), std::vector<std::string>(), "t");
    sys.output(sys.state,*this, t);
    initialize_everything_if_necessary();
    serialize_everything();
}

void Observer::initialize_everything_if_necessary()
{
    if (not(initialized))
    {
        const size_t n = stuff_to_write.size();
        size_t i = 0;
        for (auto stuff:stuff_to_write)
        {
            auto initialize_stuff = initialize.find(stuff);
            if (initialize_stuff == initialize.end())
            {
                THROW(__PRETTY_FUNCTION__, ObserverException, std::string("Simulation does not compute '") + stuff + "'");
            }
            initialize_stuff->second();
            if (i<(n-1)) flush_value();
            ++i;
        }
        flush_after_initialization();
    }
    initialized = true;
}

void Observer::serialize_everything()
{
    const size_t n = stuff_to_write.size();
    size_t i = 0;
    for (auto stuff:stuff_to_write)
    {
        auto serialize_stuff = serialize.find(stuff);
        if (serialize_stuff == serialize.end())
        {
            THROW(__PRETTY_FUNCTION__, ObserverException, std::string("Simulation does not compute '") + stuff + "'");
        }
        serialize_stuff->second();
        if (i<(n-1)) flush_value();
        ++i;
    }
    flush_after_write();
}

Observer::~Observer()
{
}
