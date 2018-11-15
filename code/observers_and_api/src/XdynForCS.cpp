#include "HistoryParser.hpp"
#include "SimServerInputs.hpp"
#include "XdynForCS.hpp"

SimServer::SimServer(const std::string& yaml_model, const std::string& solver, const double dt)
    : builder(yaml_model)
    , dt(dt)
    , stepper(builder, solver, dt)
{
}

std::vector<Res> SimServer::play_one_step(const std::string& raw_yaml)
{
    SimServerInputs simstepperinfo = parse_SimServerInputs(raw_yaml, builder.Tmax);
    return stepper.step(simstepperinfo, simstepperinfo.Dt);
}
