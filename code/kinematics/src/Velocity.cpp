#include "Velocity.hpp"
#include "KinematicsException.hpp"

Velocity::Velocity(const Point& p_, const AngularVelocityVector& w) : P(p_), vP(Eigen::Vector3d(0,0,0)), omega(w), u(vP[0])
, v(vP[1]), w(vP[2]), p(omega.x), q(omega.y), r(omega.z)
{
    if (p_.get_frame() != w.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("P is expressed in frame ") + p_.get_frame() + ", whereas omega is expressed in frame " + w.get_frame());
    }
}

std::string Velocity::get_frame() const
{
    return P.get_frame();
}

Velocity& Velocity::operator=(const Velocity& rhs)
{
    if (this != &rhs)
    {
        P = rhs.P;
        vP = rhs.vP;
        omega = rhs.omega;
        u = vP[0];
        v = vP[1];
        w = vP[2];
        p = omega.x;
        q = omega.y;
        r = omega.z;
    }
    return *this;
}

Velocity::Velocity(const Velocity& rhs) : P(rhs.P), vP(rhs.vP), omega(rhs.omega), u(vP[0])
, v(vP[1]), w(vP[2]), p(omega.x), q(omega.y), r(omega.z)
{

}

Velocity::Velocity(const Point& p_, const TranslationVelocityVector& t_, const AngularVelocityVector& w) : P(p_), vP(t_), omega(w), u(vP[0])
, v(vP[1]), w(vP[2]), p(omega.x), q(omega.y), r(omega.z)
{

}

Velocity Velocity::change_point(const Point& Q) const
{
    if (Q.get_frame() != P.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Q is expressed in frame ") + P.get_frame() + ", but velocity is expressed in frame " + Q.get_frame());
    }

    const TranslationVelocityVector vQ = vP + (P - Q).cross(omega.v);
    return Velocity(Q, vQ, omega);
}
