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
