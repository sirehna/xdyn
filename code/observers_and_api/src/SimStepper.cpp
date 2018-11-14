#include "SimStepper.hpp"
#include "simulator_api.hpp"
#include "InvalidInputException.hpp"

SimStepper::SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt)
    : sim(builder.sim)
    , solver(solver)
    , dt(dt)
{
}

SimServerInputs::SimServerInputs()
    : t(0)
    , Dt(0)
    , state_at_t(0)
    , state_history_except_last_point(0)
    , full_state_history(0)
    , commands({})
{
}

SimServerInputs::SimServerInputs(const double Dt_)
    : t(0)
    , Dt(Dt_)
    , state_at_t(0)
    , state_history_except_last_point(Dt_)
    , full_state_history(Dt_)
    , commands({})
{
}

State SimStepper::step(const SimServerInputs& infos, double Dt)
{
    const double t = infos.t;
    const std::vector<State>states = {infos.full_state_history};
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
