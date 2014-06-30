#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "GeometricTypes3d.hpp"
#include "Sim.hpp"
#include "SimulatorBuilder.hpp"
#include "solve.hpp"

SimulatorBuilder get_builder(const std::string& yaml);
Sim get_system(const std::string& yaml);
Sim get_system(const std::string& yaml, const std::string& mesh);
Sim get_system(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes);

template <typename ObserverType,
          typename StepperType> ObserverType simulate(Sim& sys, const YamlSimulatorInput& yaml, const double tstart, const double tend, const double dt)
{
    ObserverType observer(yaml);
    quicksolve<StepperType>(sys, tstart, tend, dt, observer);
    return observer;
}

template <typename ObserverType,
          typename StepperType> ObserverType simulate(const std::string& yaml, const double tstart, const double tend, const double dt)
{
    auto builder = get_builder(yaml);
    const auto parsed_yaml = builder.get_parsed_yaml();
    Sim sys = builder.build();
    return simulate<ObserverType,StepperType>(sys, parsed_yaml, tstart, tend, dt);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh);

template <typename ObserverType,
          typename StepperType> ObserverType simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt)
{
    auto builder = get_builder(yaml);
    const auto parsed_yaml = builder.get_parsed_yaml();
    const auto meshes = make_mesh_map(parsed_yaml, mesh);
    Sim sys = builder.build(meshes);
    return simulate<ObserverType,StepperType>(sys, parsed_yaml, tstart, tend, dt);
}

template <typename ObserverType,
          typename StepperType> ObserverType simulate(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    auto builder = get_builder(yaml);
    const auto parsed_yaml = builder.get_parsed_yaml();
    Sim sys = builder.build(meshes);
    return simulate<ObserverType,StepperType>(sys, parsed_yaml, tstart, tend, dt);
}

#endif
