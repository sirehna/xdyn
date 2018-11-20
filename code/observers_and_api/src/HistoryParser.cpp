#include <algorithm>

#include "History.hpp"
#include "HistoryParser.hpp"
#include "InternalErrorException.hpp"
#include "parse_history.hpp"
#include "SimServerInputs.hpp"
#include "YamlState.hpp"

void check_state_history_size(const State& state);
void check_state_history_size(const State& state)
{
    const size_t N = state.x.size();
    if (state.y.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'y' has size " << state.y.size() << " whereas state 'x' has size " << N);
    }
    if (state.z.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'z' has size " << state.z.size() << " whereas states 'x' and 'y' have size " << N);
    }
    if (state.u.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'u' has size " << state.u.size() << " whereas states 'x', 'y' and 'z' have size " << N);
    }
    if (state.v.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'v' has size " << state.v.size() << " whereas all previous states have size " << N);
    }
    if (state.w.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'w' has size " << state.w.size() << " whereas all previous states have size " << N);
    }
    if (state.p.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'p' has size " << state.p.size() << " whereas all previous states have size " << N);
    }
    if (state.q.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'q' has size " << state.q.size() << " whereas all previous states have size " << N);
    }
    if (state.r.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'r' has size " << state.r.size() << " whereas all previous states have size " << N);
    }
    if (state.qr.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'qr' has size " << state.qr.size() << " whereas all previous states have size " << N);
    }
    if (state.qi.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'qi' has size " << state.qi.size() << " whereas all previous states have size " << N);
    }
    if (state.qj.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'qj' has size " << state.qj.size() << " whereas all previous states have size " << N);
    }
    if (state.qk.size() != N)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Trying to serialize state history: all states should have the same size, but state 'qk' has size " << state.qk.size() << " whereas all the other states have size " << N);
    }
}
void operator >> (const State& state, std::vector<YamlState>& ystates);
void operator >> (const State& state, std::vector<YamlState>& ystates)
{
    const int N = (int)state.x.size();
    ystates.resize(N);
    check_state_history_size(state);
    for(int i=0 ; i<N; i++)
    {
        ystates[i].t = state.x[i].first;
        ystates[i].x = state.x[i].second;
        ystates[i].y = state.y[i].second;
        ystates[i].z = state.z[i].second;
        ystates[i].u = state.u[i].second;
        ystates[i].v = state.v[i].second;
        ystates[i].w = state.w[i].second;
        ystates[i].p = state.p[i].second;
        ystates[i].q = state.q[i].second;
        ystates[i].r = state.r[i].second;
        ystates[i].qr = state.qr[i].second;
        ystates[i].qi = state.qi[i].second;
        ystates[i].qj = state.qj[i].second;
        ystates[i].qk = state.qk[i].second;
    }
}

YamlState convert(const Res& res);
YamlState convert(const Res& res)
{
    YamlState ret;
    ret.t = res.t;
    ret.x = res.x[0];
    ret.y = res.x[1];
    ret.z = res.x[2];
    ret.u = res.x[3];
    ret.v = res.x[4];
    ret.w = res.x[5];
    ret.p = res.x[6];
    ret.q = res.x[7];
    ret.r = res.x[8];
    ret.qr = res.x[9];
    ret.qi = res.x[10];
    ret.qj = res.x[11];
    ret.qk = res.x[12];
    return ret;
}

std::string emit_state_history_json(const std::vector<Res>& states)
{
    std::vector<YamlState> yaml_states(states.size());
    std::transform(states.begin(), states.end(), yaml_states.begin(), convert);
    return encode_YamlStates(yaml_states);
}

SimServerInputs parse_SimServerInputs(const std::string& json, const double max_history_length)
{
    const YamlSimServerInputs yaml_inputs = decode_YamlSimServerInputs(json);
    return SimServerInputs(yaml_inputs, max_history_length);
}
