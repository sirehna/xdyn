#include "simulator_api.hpp"
#include "SimServerInputs.hpp"
#include "SimStepper.hpp"
#include "InvalidInputException.hpp"

SimStepper::SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt)
    : sim(builder.sim)
    , solver(solver)
    , dt(dt)
{
}

std::vector<Res> SimStepper::step(const SimServerInputs& infos, double Dt)
{
    const double t = infos.t;
    const std::vector<State>states = {infos.full_state_history};
    sim.reset_history();
    sim.set_bodystates(states);
    sim.set_command_listener(infos.commands);
    std::vector<Res> results;
    if(solver == "euler")
    {
        results = simulate<ssc::solver::EulerStepper>(sim, t, Dt, dt);
    }
    else if (solver == "rk4")
    {
        results = simulate<ssc::solver::RK4Stepper>(sim, t, Dt, dt);
    }
    else if (solver == "rkck")
    {
        results = simulate<ssc::solver::RKCK>(sim, t, Dt, dt);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "unknown solver");
    }
    return results;
}
