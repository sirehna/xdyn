#include "Velocity.hpp"
#include "KinematicsException.hpp"

Velocity::Velocity(const Point& p_, const AngularVelocityVector& w) : P(p_), omega(w), u(P.v[0])
, v(P.v[1]), w(P.v[2]), p(omega.v[0]), q(omega.v[1]), r(omega.v[2])
{
    if (p_.get_frame() != w.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("P is expressed in frame") + p_.get_frame() + ", whereas omega is expressed in frame " + w.get_frame());
    }
}

Velocity Velocity::change_point(const Point& Q) const
{
    if (Q.get_frame() != P.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Q is expressed in frame ") + P.get_frame() + ", but velocity is expressed in frame " + Q.get_frame());
    }
    return *this;
}
