#include "YamlState.hpp"

YamlHistory::YamlHistory()
    : values()
    , tau()
{
}

YamlHistory::YamlHistory(const double) // To keep AbstractStates happy
    : values()
    , tau()
{
}

YamlState::YamlState()
    : AbstractStates<YamlHistory>()
    , t()
{
}


bool operator == (const YamlState& state1, const YamlState& state2)
{
    bool eq_t = state1.t == state2.t;
    bool eq_x = state1.x == state2.x;
    bool eq_y = state1.y == state2.y;
    bool eq_z = state1.z == state2.z;
    bool eq_u = state1.u == state2.u;
    bool eq_v = state1.v == state2.v;
    bool eq_w = state1.w == state2.w;
    bool eq_p = state1.p == state2.p;
    bool eq_q = state1.q == state2.q;
    bool eq_r = state1.r == state2.r;
    bool eq_qr = state1.qr == state2.qr;
    bool eq_qi = state1.qi == state2.qi;
    bool eq_qj = state1.qj == state2.qj;
    bool eq_qk = state1.qk == state2.qk;


    return eq_t && eq_x && eq_y && eq_z && eq_u && eq_v && eq_w && eq_p && eq_q && eq_r && eq_qr && eq_qi && eq_qj && eq_qk;


}

bool operator == (const YamlHistory& h1, const YamlHistory& h2)
{
    return (h1.tau == h2.tau) && (h1.values == h2.values);

}
