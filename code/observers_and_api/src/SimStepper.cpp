#include "SimStepper.hpp"
#include "simulator_api.hpp"


SimStepper::SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt):
sim(builder.sim),
solver(solver),
dt(dt)
{
}

State SimStepper::step(const State& state, const double t, const double Dt)
{
    const std::vector<State>states =  {state};
    sim.set_bodystates(states);
    const std::vector<Res> results = simulate<ssc::solver::EulerStepper>(sim, t, Dt, dt);
    sim.get_bodies().front()->update_body_states(results.back().x, results.back().t);
    return State(sim.get_bodies().front()->get_states());
}
