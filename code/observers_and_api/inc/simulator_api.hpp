#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "EverythingObserver.hpp"
#include "Sim.hpp"
#include "SimulatorBuilder.hpp"
#include "SimObserver.hpp"
#include <ssc/solver.hpp>

struct YamlSimulatorInput;

Sim get_system(const std::string& yaml, const double t0);
Sim get_system(const std::string& yaml, const std::string& mesh, const double t0);
Sim get_system(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double t0);
Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh, const double t0);
Sim get_system(const YamlSimulatorInput& yaml, const double t0);
Sim get_system(const YamlSimulatorInput& yaml, const std::string& mesh, const double t0);
Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0);
Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0, ssc::data_source::DataSource& commands);
Sim get_system(const std::string& input, const VectorOfVectorOfPoints& mesh, const double t0, ssc::data_source::DataSource& commands);
Sim get_system(const std::string& input, const std::string& mesh, const double t0, ssc::data_source::DataSource& commands);
Sim get_system(const YamlSimulatorInput& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double t0);

template <typename StepperType> std::vector<Res> simulate(Sim& sys, const double tstart, const double tend, const double dt)
{
    EverythingObserver observer;
    ssc::solver::quicksolve<StepperType>(sys, tstart, tend, dt, observer);
    return observer.get();
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, tstart);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh);

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, mesh, tstart);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt, ssc::data_source::DataSource& commands)
{
    Sim sys = get_system(yaml, mesh, tstart, commands);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, meshes, tstart);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, tstart);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    Sim sys = get_system(yaml, meshes, tstart);
    return simulate<StepperType>(sys, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const VectorOfVectorOfPoints& mesh, const double tstart, const double tend, const double dt)
{
    std::map<std::string, VectorOfVectorOfPoints> meshes;
    meshes[yaml.bodies.front().name] = mesh;
    return simulate<StepperType>(yaml, meshes, tstart, tend, dt);
}

template <typename StepperType> std::vector<Res> simulate(const YamlSimulatorInput& yaml, const VectorOfVectorOfPoints& mesh, const double tstart, const double tend, const double dt, ssc::data_source::DataSource& commands)
{
    Sim sys = get_system(yaml, mesh, tstart, commands);
    SimObserver observer;
    ssc::solver::quicksolve<StepperType>(sys, tstart, tend, dt, observer);
    return observer.get();
}

#endif
