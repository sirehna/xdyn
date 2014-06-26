#include "Velocity.hpp"
#include "KinematicsException.hpp"

Velocity::Velocity(const Point& p_, const AngularVelocityVector& w) : P(p_), vP(p_.get_frame(), Eigen::Vector3d(0,0,0)), omega(w)
{
    if (p_.get_frame() != w.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("P is expressed in frame ") + p_.get_frame() + ", whereas omega is expressed in frame " + w.get_frame());
    }
}

Point Velocity::get_point() const
{
    return P;
}

AngularVelocityVector Velocity::get_angular_velocity() const
{
    return omega;
}

TranslationVelocityVector Velocity::get_translation_velocity() const
{
    return vP;
}

std::string Velocity::get_frame() const
{
    return P.get_frame();
}

Velocity& Velocity::operator=(Velocity rhs)
{
    swap(rhs);
    return *this;
}

Velocity::Velocity(const Velocity& rhs) : P(rhs.P), vP(rhs.vP), omega(rhs.omega)
{

}

Velocity::Velocity(const Point& p_, const TranslationVelocityVector& t_, const AngularVelocityVector& w) : P(p_), vP(t_), omega(w)
{
}

Velocity Velocity::change_point(const Point& Q) const
{
    if (Q.get_frame() != P.get_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Q is expressed in frame ") + P.get_frame() + ", but velocity is expressed in frame " + Q.get_frame());
    }

    const TranslationVelocityVector vQ(vP.get_frame(), vP.v + (P - Q).cross(omega.v));
    return Velocity(Q, vQ, omega);
}

void Velocity::swap(Velocity& other)
{
    using std::swap; // For argument-dependent lookup (ADL): not strictly necessary here but good practice
    swap(P, other.P);
    swap(vP, other.vP);
    swap(omega, other.omega);
}

void swap(Velocity& a, Velocity& b) // provide non-member for ADL
{
    a.swap(b);
}
