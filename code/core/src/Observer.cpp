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

bool Observer::is_initialized() const
{
    return initialized;
}

void Observer::observe(const Sim& sys, const double t)
{
    write(t, std::vector<std::string>(), "t");
    sys.output(sys.state,*this);
    initialize_everything_if_necessary();
    serialize_everything();
}

void Observer::initialize_everything_if_necessary()
{
    if (not(initialized))
    {
        for (auto stuff:stuff_to_write)
        {
            auto initialize_stuff = initialize.find(stuff);
            if (initialize_stuff == initialize.end())
            {
                THROW(__PRETTY_FUNCTION__, ObserverException, std::string("Simulation does not compute '") + stuff + "'");
            }
        }
        for (auto stuff:stuff_to_write) initialize[stuff]();
        flush_after_initialization();
    }
    initialized = true;
}

void Observer::serialize_everything()
{
    for (auto stuff:stuff_to_write) serialize[stuff]();
    flush_after_write();
}

Observer::~Observer()
{
}
