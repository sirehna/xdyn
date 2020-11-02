/*
 * XdynForME.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */


#include "SimServerInputs.hpp"
#include "XdynForME.hpp"

XdynForME::XdynForME(const std::string& yaml_model) :builder(yaml_model)
{
}

double XdynForME::get_Tmax() const
{
    return builder.Tmax;
}

StateType XdynForME::calculate_dx_dt(const SimServerInputs& server_inputs)
{
    const double t = server_inputs.t;
    builder.sim.set_bodystates(server_inputs.state_history_except_last_point);
    builder.sim.set_command_listener(server_inputs.commands);

    StateType dx_dt(13, 0);
    builder.sim.dx_dt(server_inputs.state_at_t, dx_dt, t);

    return dx_dt;
}
