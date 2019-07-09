/*
 * Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "Observer.hpp"
#include "InvalidInputException.hpp"
#include "Sim.hpp"
#include "SurfaceElevationGrid.hpp"

Observer::Observer(const std::vector<std::string>& data_) : initialized(false), requested_serializations(data_), serialize(), initialize()
{
}

std::function<void()> Observer::get_serializer(const SurfaceElevationGrid& , const DataAddressing& )
{
    return [](){};
}

std::function<void()> Observer::get_initializer(const SurfaceElevationGrid& , const DataAddressing& )
{
    return [](){};
}

void Observer::before_write()
{
}

void Observer::observe(const Sim& sys, const double t)
{
    write(t, DataAddressing(std::vector<std::string>(1,"t"), "t"));
    sys.output(sys.state,*this, t);
    initialize_serialization_of_requested_variables();
    serialize_stuff_to_write();
}

void Observer::observe_everything(const Sim& sys, const double t)
{
    write(t, DataAddressing(std::vector<std::string>(1,"t"), "t"));
    sys.output(sys.state,*this, t);
    for (const auto& initializer:initialize) initializer.second();
    flush_after_initialization();
    for (const auto& serializer:serialize) serializer.second();
    flush_after_write();
}

void Observer::initialize_serialization_of_requested_variables()
{
    if (not(initialized))
    {
        const size_t n = requested_serializations.size();
        size_t i = 0;
        for (auto stuff:requested_serializations)
        {
            auto initialize_stuff = initialize.find(stuff);
            if (initialize_stuff == initialize.end())
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "In the 'outputs' section of the YAML file, you asked for '" << stuff << "', but it is not computed: maybe it is misspelt or the corresponding model is not in the YAML.");
            }
            initialize_stuff->second();
            if (i<(n-1)) flush_value_during_initialization();
            ++i;
        }
        flush_after_initialization();
    }
    initialized = true;
}

void Observer::serialize_stuff_to_write()
{
    const size_t n = requested_serializations.size();
    size_t i = 0;
    before_write();
    for (auto stuff:requested_serializations)
    {
        auto serialize_stuff = serialize.find(stuff);
        if (serialize_stuff == serialize.end())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "In the 'outputs' section of the YAML file, you asked for '" << stuff << "', but it is not computed: maybe it is misspelt or the corresponding model is not in the YAML.");
        }
        serialize_stuff->second();
        if (i<(n-1)) flush_value_during_write();
        ++i;
    }
    flush_after_write();
}

Observer::~Observer()
{
}

void Observer::flush_value_during_initialization()
{
    flush_value_during_write();
}

void Observer::write_before_simulation(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& , const DataAddressing& )
{}
