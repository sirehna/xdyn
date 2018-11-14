#include "HistoryParser.hpp"
#include "History.hpp"
#include "YamlState.hpp"
#include "parse_history.hpp"
#include "SimStepper.hpp"


HistoryParser::HistoryParser(const double Tmax)
: Tmax(Tmax)
{

}


void operator >> (const YamlState& ystate, State& state);
void operator >> (const YamlState& ystate, State& state)
{
    const double t = ystate.t;
    for(size_t i=0 ; i<ystate.x.values.size() ; i++)
    {
        state.x.record(t-ystate.x.tau[i], ystate.x.values[i]);
        state.y.record(t-ystate.y.tau[i], ystate.y.values[i]);
        state.z.record(t-ystate.z.tau[i], ystate.z.values[i]);
        state.u.record(t-ystate.u.tau[i], ystate.u.values[i]);
        state.v.record(t-ystate.v.tau[i], ystate.v.values[i]);
        state.w.record(t-ystate.w.tau[i], ystate.w.values[i]);
        state.p.record(t-ystate.p.tau[i], ystate.p.values[i]);
        state.q.record(t-ystate.q.tau[i], ystate.q.values[i]);
        state.r.record(t-ystate.r.tau[i], ystate.r.values[i]);
        state.qr.record(t-ystate.qr.tau[i], ystate.qr.values[i]);
        state.qi.record(t-ystate.qi.tau[i], ystate.qi.values[i]);
        state.qj.record(t-ystate.qj.tau[i], ystate.qj.values[i]);
        state.qk.record(t-ystate.qk.tau[i], ystate.qk.values[i]);
    }
}


void operator >> (const State& state, YamlState& ystate);
void operator >> (const State& state, YamlState& ystate)
{
    const double t = state.x[-1].first;
    for(int i=0 ; i<(int)state.x.size(); i++)
    {
        ystate.x.tau.push_back(t-state.x[i].first);
        ystate.x.values.push_back(state.x[i].second);
        ystate.y.tau.push_back(t-state.y[i].first);
        ystate.y.values.push_back(state.y[i].second);
        ystate.z.tau.push_back(t-state.z[i].first);
        ystate.z.values.push_back(state.z[i].second);
        ystate.u.tau.push_back(t-state.u[i].first);
        ystate.u.values.push_back(state.u[i].second);
        ystate.v.tau.push_back(t-state.v[i].first);
        ystate.v.values.push_back(state.v[i].second);
        ystate.w.tau.push_back(t-state.w[i].first);
        ystate.w.values.push_back(state.w[i].second);
        ystate.p.tau.push_back(t-state.p[i].first);
        ystate.p.values.push_back(state.p[i].second);
        ystate.q.tau.push_back(t-state.q[i].first);
        ystate.q.values.push_back(state.q[i].second);
        ystate.r.tau.push_back(t-state.r[i].first);
        ystate.r.values.push_back(state.r[i].second);
        ystate.qr.tau.push_back(t-state.qr[i].first);
        ystate.qr.values.push_back(state.qr[i].second);
        ystate.qi.tau.push_back(t-state.qi[i].first);
        ystate.qi.values.push_back(state.qi[i].second);
        ystate.qj.tau.push_back(t-state.qj[i].first);
        ystate.qj.values.push_back(state.qj[i].second);
        ystate.qk.tau.push_back(t-state.qk[i].first);
        ystate.qk.values.push_back(state.qk[i].second);
    }
}



State HistoryParser::parse_state_history_yaml(const std::string& yaml, const double Tmax)
{
    const YamlState ystate=parse_history_yaml(yaml);
    State s(Tmax);
    ystate >> s;
    return s;
}

std::string  HistoryParser::emit_state_history_yaml(const State& state)
{
    YamlState ystate;
    state >> ystate;
    return generate_history_yaml(ystate);
}

YamlHistory extract_all_except_last(const YamlHistory& his, const double Tmax);
YamlHistory extract_all_except_last(const YamlHistory& his, const double Tmax)
{
    const size_t n = his.values.size();
    YamlHistory ret(Tmax);
    if (n>1)
    {
        ret.tau.resize(n-1);
        ret.values.resize(n-1);
    }
    for (size_t i = 0 ; i < n-1 ; ++i)
    {
        ret.tau[i] = his.tau[i];
        ret.values[i] = his.values[i];
    }
    return ret;
}

double extract_last(const YamlHistory& his);
double extract_last(const YamlHistory& his)
{
    if (not(his.values.empty())) return his.values.back();
                                 return 0;
}

YamlState get_all_states_except_last(const YamlState state, const double Tmax);
YamlState get_all_states_except_last(const YamlState state, const double Tmax)
{
    YamlState ret;
    ret.x = extract_all_except_last(state.x, Tmax);
    ret.y = extract_all_except_last(state.y, Tmax);
    ret.z = extract_all_except_last(state.z, Tmax);
    ret.u = extract_all_except_last(state.u, Tmax);
    ret.v = extract_all_except_last(state.v, Tmax);
    ret.w = extract_all_except_last(state.w, Tmax);
    ret.p = extract_all_except_last(state.p, Tmax);
    ret.q = extract_all_except_last(state.q, Tmax);
    ret.r = extract_all_except_last(state.r, Tmax);
    ret.qr = extract_all_except_last(state.qr, Tmax);
    ret.qi = extract_all_except_last(state.qi, Tmax);
    ret.qj = extract_all_except_last(state.qj, Tmax);
    ret.qk = extract_all_except_last(state.qk, Tmax);
    ret.t = state.t;
    if (not(ret.x.tau.empty()))
    {
        ret.t -= ret.x.tau.back();
    }
    return ret;
}

StateType get_last_state(const YamlState state);
StateType get_last_state(const YamlState state)
{
    StateType ret(13,0);
    ret[0] = extract_last(state.x);
    ret[1] = extract_last(state.y);
    ret[2] = extract_last(state.z);
    ret[3] = extract_last(state.u);
    ret[4] = extract_last(state.v);
    ret[5] = extract_last(state.w);
    ret[6] = extract_last(state.p);
    ret[7] = extract_last(state.q);
    ret[8] = extract_last(state.r);
    ret[9] = extract_last(state.qr);
    ret[10] = extract_last(state.qi);
    ret[11] = extract_last(state.qj);
    ret[12] = extract_last(state.qk);
    return ret;
}

void operator << (SimServerInputs& inputs, const YamlSimServerInputs& yaml_inputs);
void operator << (SimServerInputs& inputs, const YamlSimServerInputs& yaml_inputs)
{
    inputs.Dt = yaml_inputs.Dt;
    yaml_inputs.state >> inputs.full_state_history;
    const YamlState all_states_except_last = get_all_states_except_last(yaml_inputs.state, inputs.Dt);
    inputs.state_at_t = get_last_state(yaml_inputs.state);
    all_states_except_last >> inputs.state_history_except_last_point;
    inputs.commands = yaml_inputs.commands;
}


SimServerInputs HistoryParser::parse_SimServerInputs(const std::string& yaml) const
{
    YamlSimServerInputs yaml_inputs = parse_YamlSimServerInputs(yaml);
    SimServerInputs inputs(yaml_inputs.Dt);
    inputs << yaml_inputs;
    return inputs;
}
