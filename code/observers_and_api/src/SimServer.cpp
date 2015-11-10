#include "SimServer.hpp"

SimServer::SimServer(const std::string& yaml_model, const std::string& solver, const double dt)
    : builder(yaml_model)
    , dt(dt)
    , stepper(builder, solver, dt)
    , parser(builder.Tmax)
{
}


std::string SimServer::play_one_step(const std::string& raw_yaml)
{
    SimStepperInfos simstepperinfo = parser.get_simstepperinfo(raw_yaml);
    State result=stepper.step(simstepperinfo, simstepperinfo.Dt);
    return parser.emit_state_history_yaml(result);
}
