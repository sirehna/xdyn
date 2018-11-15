/*
 * SimServerInputs.cpp
 *
 *  Created on: Nov 15, 2018
 *      Author: cady
 */


#include "SimServerInputs.hpp"
#include "YamlSimServerInputs.hpp"
#include "YamlState.hpp"

void record(State& state, const YamlState& ystate);
void record(State& state, const YamlState& ystate)
{
    state.x.record(ystate.t, ystate.x);
    state.y.record(ystate.t, ystate.y);
    state.z.record(ystate.t, ystate.z);
    state.u.record(ystate.t, ystate.u);
    state.v.record(ystate.t, ystate.v);
    state.w.record(ystate.t, ystate.w);
    state.p.record(ystate.t, ystate.p);
    state.q.record(ystate.t, ystate.q);
    state.r.record(ystate.t, ystate.r);
    state.qr.record(ystate.t, ystate.qr);
    state.qi.record(ystate.t, ystate.qi);
    state.qj.record(ystate.t, ystate.qj);
    state.qk.record(ystate.t, ystate.qk);
}

SimServerInputs::SimServerInputs(const YamlSimServerInputs& server_inputs, const double max_history_length)
    : t(0)
    , Dt(server_inputs.Dt)
    , state_at_t(13, 0)
    , state_history_except_last_point(max_history_length)
    , full_state_history(max_history_length)
    , commands(server_inputs.commands)
{
    for(size_t i=0 ; i< (server_inputs.states.size()-1) ; i++)
    {
        record(state_history_except_last_point, server_inputs.states[i]);
        record(full_state_history, server_inputs.states[i]);
    }
    if (not(server_inputs.states.empty()))
    {
        state_at_t[0] = server_inputs.states.back().x;
        state_at_t[1] = server_inputs.states.back().y;
        state_at_t[2] = server_inputs.states.back().z;
        state_at_t[3] = server_inputs.states.back().u;
        state_at_t[4] = server_inputs.states.back().v;
        state_at_t[5] = server_inputs.states.back().w;
        state_at_t[6] = server_inputs.states.back().p;
        state_at_t[7] = server_inputs.states.back().q;
        state_at_t[8] = server_inputs.states.back().r;
        state_at_t[9] = server_inputs.states.back().qr;
        state_at_t[10] = server_inputs.states.back().qi;
        state_at_t[11] = server_inputs.states.back().qj;
        state_at_t[12] = server_inputs.states.back().qk;
        record(full_state_history, server_inputs.states.back());
    }
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
