#include "SimServer.hpp"

SimServer::SimServer(const std::string& yaml_model, const std::string& solver, const double dt, const double Dt)
    : builder(yaml_model)
    , dt(dt)
    , stepper(builder, solver, dt)
    , parser(builder.Tmax)
    , Dt(Dt)
{
}


std::string SimServer::play_one_step(const std::string& raw_yaml)
{
    SimStepperInfos simstepperinfo = parser.get_simstepperinfo(raw_yaml);
    return parser.emit_state_history_yaml(stepper.step(simstepperinfo, Dt));

}
