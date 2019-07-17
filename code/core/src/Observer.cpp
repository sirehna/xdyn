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
    initialize_serialization_of_requested_variables(requested_serializations);
    serialize_requested_variables(requested_serializations);
}

std::vector<std::string> all_variables(std::map<std::string, std::function<void()> >& map);
std::vector<std::string> all_variables(std::map<std::string, std::function<void()> >& map)
{
    std::vector<std::string> ret;
    std::transform(map.begin(), map.end(), std::back_inserter(ret), [](const std::pair<std::string, std::function<void()> >& p){return p.first;});
    return ret;
}

void Observer::observe_everything(const Sim& sys, const double t)
{
    write(t, DataAddressing(std::vector<std::string>(1,"t"), "t"));
    sys.output(sys.state,*this, t);
    const auto all_vars = all_variables(initialize);
    initialize_serialization_of_requested_variables(all_vars);
    serialize_requested_variables(all_vars);
}

void Observer::initialize_serialization_of_requested_variables(const std::vector<std::string>& variables_to_serialize)
{
    if (not(initialized))
    {
        const size_t n = variables_to_serialize.size();
        size_t i = 0;
        for (auto stuff:variables_to_serialize)
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

void Observer::serialize_requested_variables(const std::vector<std::string>& variables_to_serialize)
{
    const size_t n = variables_to_serialize.size();
    size_t i = 0;
    before_write();
    for (auto variable_name:variables_to_serialize)
    {
        auto serialization_functor = serialize.find(variable_name);
        if (serialization_functor == serialize.end())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "In the 'outputs' section of the YAML file, you asked for '" << variable_name << "', but it is not computed: maybe it is misspelt or the corresponding model is not in the YAML.");
        }
        serialization_functor->second();
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
