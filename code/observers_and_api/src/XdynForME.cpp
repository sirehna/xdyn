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

std::string XdynForME::calculate_dx_dt(const std::string& raw_yaml)
{
    try
    {
        SimServerInputs server_inputs = parse_SimServerInputs(raw_yaml, builder.Tmax);
        const double t = server_inputs.t;
        const std::vector<State>states(1, server_inputs.state_history_except_last_point);
        builder.sim.set_bodystates(states);
        builder.sim.set_command_listener(server_inputs.commands);

        StateType dx_dt(13, 0);
        builder.sim.dx_dt(server_inputs.state_at_t, dx_dt, t);

        std::stringstream ss;

        ss << "{"
           << "\"dx_dt\": "  << dx_dt[0] << ","
           << "\"dy_dt\": "  << dx_dt[1] << ","
           << "\"dz_dt\": "  << dx_dt[2] << ","
           << "\"du_dt\": "  << dx_dt[3] << ","
           << "\"dv_dt\": "  << dx_dt[4] << ","
           << "\"dw_dt\": "  << dx_dt[5] << ","
           << "\"dp_dt\": "  << dx_dt[6] << ","
           << "\"dq_dt\": "  << dx_dt[7] << ","
           << "\"dr_dt\": "  << dx_dt[8] << ","
           << "\"dqr_dt\": " << dx_dt[9] << ","
           << "\"dqi_dt\": " << dx_dt[10] << ","
           << "\"dqj_dt\": " << dx_dt[11] << ","
           << "\"dqk_dt\": " << dx_dt[12]
           << "}";

        return ss.str();
    }
    catch(std::exception& e)
    {
        return std::string("{\"error\": \"") + e.what() + "\"}";
    }
}
