#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "GeometricTypes3d.hpp"
#include "Sim.hpp"
#include "solve.hpp"

Sim get_system(const std::string& yaml);
Sim get_system(const std::string& yaml, const std::string& mesh);
Sim get_system(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes);

template <typename ObserverType,
          typename StepperType> ObserverType simulate(Sim& sys, const double tstart, const double tend, const double dt)
{
    ObserverType observer(sys);
    quicksolve<StepperType>(sys, tstart, tend, dt, observer);
    return observer;
}

template <typename ObserverType,
          typename StepperType> ObserverType simulate(const std::string& yaml, const double tstart, const double tend, const double dt)
{
    auto sys = get_system(yaml);
    return simulate<ObserverType,StepperType>(sys, tstart, tend, dt);
}

template <typename ObserverType,
          typename StepperType> ObserverType simulate(const std::string& yaml, const std::string& mesh, const double tstart, const double tend, const double dt)
{
    auto sys = get_system(yaml,mesh);
    return simulate<ObserverType,StepperType>(sys, tstart, tend, dt);
}

template <typename ObserverType,
          typename StepperType> ObserverType simulate(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes, const double tstart, const double tend, const double dt)
{
    auto sys = get_system(yaml,meshes);
    return simulate<ObserverType,StepperType>(sys, tstart, tend, dt);
}

#endif
