#include "SimStepper.hpp"
#include "simulator_api.hpp"
#include "InvalidInputException.hpp"

SimStepper::SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt)
    : sim(builder.sim)
    , solver(solver)
    , dt(dt)
{
}

SimStepperInfos::SimStepperInfos()
    : t(0)
    , Dt(0)
    , state(0)
    , commands({})
{
}

State SimStepper::step(const SimStepperInfos& infos, double Dt)
{
    const double t = infos.t;
    std::vector<State>states =  {infos.state};
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
    sim.get_bodies().front()->update_body_states(results.back().x, results.back().t);
    return State(sim.get_bodies().front()->get_states());
}
