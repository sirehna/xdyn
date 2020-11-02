#include <functional>

#include "InvalidInputException.hpp"
#include "SimServerInputs.hpp"
#include "SimStepper.hpp"
#include "simulator_api.hpp"

SimStepper::SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt)
    : sim(builder.sim)
    , solver(solver)
    , dt(dt)
{
}

YamlState convert_without_angles(const Res& res);
YamlState convert_without_angles(const Res& res)
{
    YamlState ret;
    ret.t     = res.t;
    ret.x     = res.x[0];
    ret.y     = res.x[1];
    ret.z     = res.x[2];
    ret.u     = res.x[3];
    ret.v     = res.x[4];
    ret.w     = res.x[5];
    ret.p     = res.x[6];
    ret.q     = res.x[7];
    ret.r     = res.x[8];
    ret.qr    = res.x[9];
    ret.qi    = res.x[10];
    ret.qj    = res.x[11];
    ret.qk    = res.x[12];
    ret.phi   = 0;
    ret.theta = 0;
    ret.psi   = 0;
    ret.extra_observations = res.extra_observations;
    return ret;
}

std::function<YamlState(const Res&)> convert_with_angles(const BodyPtr& body);
std::function<YamlState(const Res&)> convert_with_angles(const BodyPtr& body)
{
    return [&body](const Res& res)
            {
                YamlState ret = convert_without_angles(res);
                const State new_state(ret,0);
                body->set_states_history(new_state);
                const auto angles = body->get_states().get_angles();
                ret.phi = angles.phi;
                ret.theta = angles.theta;
                ret.psi = angles.psi;
                return ret;
            };
}

std::vector<YamlState> SimStepper::step(const SimServerInputs& infos, double Dt)
{
    const double tstart = infos.t;
    sim.reset_history();
    sim.set_bodystates(infos.full_state_history);
    sim.set_command_listener(infos.commands);
    std::vector<Res> results;
    if(solver == "euler")
    {
        results = simulate<ssc::solver::EulerStepper>(sim, tstart, tstart+Dt, dt);
    }
    else if (solver == "rk4")
    {
        results = simulate<ssc::solver::RK4Stepper>(sim, tstart, tstart+Dt, dt);
    }
    else if (solver == "rkck")
    {
        results = simulate<ssc::solver::RKCK>(sim, tstart, tstart+Dt, dt);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "unknown solver");
    }
    std::vector<YamlState> ret(results.size());
    if (not(sim.get_bodies().empty()))
    {
        std::transform(results.begin(), results.end(), ret.begin(), convert_with_angles(sim.get_bodies().at(0)));
    }
    else
    {
        std::transform(results.begin(), results.end(), ret.begin(), convert_without_angles);
    }
    return ret;
}
