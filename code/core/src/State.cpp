#include "State.hpp"

State::State(const AbstractStates<double>& vals,
             const double t0) :
  AbstractStates<History>()
{
    x.record(t0, vals.x);
    y.record(t0, vals.y);
    z.record(t0, vals.z);
    u.record(t0, vals.u);
    v.record(t0, vals.v);
    w.record(t0, vals.w);
    p.record(t0, vals.p);
    q.record(t0, vals.q);
    r.record(t0, vals.r);
    qr.record(t0, vals.qr);
    qi.record(t0, vals.qi);
    qj.record(t0, vals.qj);
    qk.record(t0, vals.qk);
}

State::State(const double Tmax) :
        AbstractStates<History>(Tmax)
{
}

State::State(const StateType& state_values, const double t) :
        AbstractStates<History>()
{
    x.record(t, state_values[0]);
    y.record(t, state_values[1]);
    z.record(t, state_values[2]);
    u.record(t, state_values[3]);
    v.record(t, state_values[4]);
    w.record(t, state_values[5]);
    p.record(t, state_values[6]);
    q.record(t, state_values[7]);
    r.record(t, state_values[8]);
    qr.record(t, state_values[9]);
    qi.record(t, state_values[10]);
    qj.record(t, state_values[11]);
    qk.record(t, state_values[12]);
}
