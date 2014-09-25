#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "GeometricTypes3d.hpp"
#include "Sim.hpp"
#include "SimulatorBuilder.hpp"
#include "SimObserver.hpp"
#include <ssc/solver.hpp>

class YamlSimulatorInput;

SimulatorBuilder get_builder(const YamlSimulatorInput& yaml);
Sim get_system(const std::string& yaml);
Sim get_system(const std::string& yaml, const std::string& mesh);
Sim get_system(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes);
Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh);
Sim get_system(const YamlSimulatorInput& yaml);
Sim get_system(const YamlSimulatorInput& yaml, const std::string& mesh);
Sim get_system(const YamlSimulatorInput& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes);

template <typename StepperType> std::vector<Res> simulate(Sim& sys, const double tstart, const double tend, const double dt)
{
    SimObserver observer;
    ssc::solver::quicksolve<StepperType>(sys, tstart, tend, dt, observer);
    return observer.get();
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh);

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, mesh);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, meshes);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, meshes);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const VectorOfVectorOfPoints& mesh, const double tstart, const double tend, const double dt)
{
    std::map<std::string, VectorOfVectorOfPoints> meshes;
    meshes[yaml.bodies.front().name] = mesh;
    return simulate<StepperType>(yaml, meshes, tstart, tend, dt);
}

#endif
