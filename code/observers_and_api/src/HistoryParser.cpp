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
    ystate>>s;
    return s;
}

std::string  HistoryParser::emit_state_history_yaml(const State& state)
{
    YamlState ystate;
    state>>ystate;
    return generate_history_yaml(ystate);
}

void operator << (SimStepperInfos& info, const YamlSimStepperInfo& yinfo);
void operator << (SimStepperInfos& info, const YamlSimStepperInfo& yinfo)
{
    info.Dt = yinfo.Dt;
    yinfo.state>>info.state;
    info.commands = yinfo.commands;

}


SimStepperInfos HistoryParser::get_simstepperinfo(const std::string& yaml)
{
    YamlSimStepperInfo yinfo = get_yamlsimstepperinfo(yaml);
    SimStepperInfos infos;
    infos<<yinfo;
    return infos;
}
