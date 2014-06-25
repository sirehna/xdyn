/*
 * Wrench.cpp
 *
 *  Created on: 24 avr. 2014
 *      Author: cady
 */

#include "Wrench.hpp"
#include "KinematicsException.hpp"
#include "almost_equal.hpp"
#include "Transform.hpp"

#include <sstream>

bool equal(const Point& P, const Point& Q);
bool equal(const Point& P, const Point& Q)
{
    if (P.get_frame() != Q.get_frame()) return false;
    if (not(almost_equal(P.x(), Q.x())))    return false;
    if (not(almost_equal(P.y(), Q.y())))    return false;
    if (not(almost_equal(P.z(), Q.z())))    return false;
                                        return true;
}

Eigen::Vector3d transport(const Eigen::Vector3d& Ma, const Point& a, const Point& b, const Eigen::Vector3d& f);
Eigen::Vector3d transport(const Eigen::Vector3d& Ma, const Point& a, const Point& b, const Eigen::Vector3d& f)
{
    return Ma + (a-b).cross(f);
}


Wrench::Wrench(const Point& P_) : force(Eigen::Vector3d(0,0,0)),
                                  torque(Eigen::Vector3d(0,0,0)),
                                  P(P_)
{
}

Wrench::Wrench() : force(Eigen::Vector3d()),
                   torque(Eigen::Vector3d()),
                   P(Point())
{
}

Wrench::Wrench(const Point& P_, const Eigen::Vector3d& force_, const Eigen::Vector3d& torque_) : force(force_),
                                                                                                 torque(torque_),
                                                                                                 P(P_)
{
}


Wrench::Wrench(const Wrench& rhs) : force(rhs.force),
                                  torque(rhs.torque),
                                  P(rhs.P)
{
}

Wrench& Wrench::operator=(const Wrench& rhs)
{
    if (this != &rhs)
    {
        force = rhs.force;
        torque = rhs.torque;
        P = rhs.P;
    }
    return *this;
}

Wrench Wrench::operator+(const Wrench& rhs) const
{
    if (not(equal(P, rhs.P)))
    {
        std::stringstream ss;
        ss << "Wrenches are not expressed at the same point: lhs is at "
           << P
           << ", whereas rhs is at " << rhs.P;
        THROW(__PRETTY_FUNCTION__, KinematicsException, ss.str());
    }
    return Wrench(P, force + rhs.force, torque + rhs.torque);
}

Wrench Wrench::operator-(const Wrench& rhs) const
{
    if (not(equal(P, rhs.P)))
    {
        std::stringstream ss;
        ss << "Wrenches are not expressed at the same point: lhs is at "
           << P
           << ", whereas rhs is at " << rhs.P;
        THROW(__PRETTY_FUNCTION__, KinematicsException, ss.str());
    }
    return Wrench(P, force - rhs.force, torque - rhs.torque);
}

Wrench Wrench::change_point_of_application(const Point& Q) const
{
    if (P.get_frame() != Q.get_frame())
    {
        std::stringstream ss;
        ss << "Points of application are not expressed in the same frame: this is in "
           << P.get_frame()
           << ", whereas Q is in " << Q.get_frame();
        THROW(__PRETTY_FUNCTION__, KinematicsException, ss.str());
    }
    return Wrench(Q, force, transport(torque,P, Q, force));
}

Wrench Wrench::change_frame_but_keep_ref_point(const kinematics::Transform& T) const
{
    if (P.get_frame() != T.get_from_frame())
    {
        std::stringstream ss;
        ss << "Frames mismatch: Wrench is expressed in "
           << P.get_frame()
           << ", but transform is " << T;
        THROW(__PRETTY_FUNCTION__, KinematicsException, ss.str());
    }
    const RotationMatrix R = T.get_rot();
    return Wrench(P, R*force, R*torque);
}

Wrench Wrench::change_ref_point_then_change_frame(const kinematics::Transform& T) const
{
    if (P.get_frame() != T.get_from_frame())
    {
        std::stringstream ss;
        ss << "Frames mismatch: Wrench is expressed in "
           << P.get_frame()
           << ", but transform is " << T;
        THROW(__PRETTY_FUNCTION__, KinematicsException, ss.str());
    }
    const Point Q = T.get_point();
    const RotationMatrix R = T.get_rot();
    return Wrench(Q, R*force, R*transport(torque,P, Q, force));
}

Point Wrench::get_point() const
{
    return P;
}

std::string Wrench::get_frame() const
{
    return P.get_frame();
}

Wrench Wrench::operator*(const double lambda) const
{
    return Wrench(P, lambda*force, lambda*torque);
}

Wrench operator*(const double lambda, const Wrench& w)
{
    return w*lambda;
}

std::ostream& operator<<(std::ostream& os, const Wrench& w)
{
    os << "force: [" << w.force.transpose() << "], torque: [" << w.torque.transpose() << "], point: " << w.get_point();
    return os;
}

Eigen::Matrix<double, 6, 1> Wrench::to_vector() const
{
    double v[6] = {X(),Y(),Z(),K(),M(),N()};
    return Eigen::Map<Eigen::Matrix<double, 6, 1> >(v);
}
