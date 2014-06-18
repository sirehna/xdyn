/*
 * UnsafeWrench.cpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#include "UnsafeWrench.hpp"

UnsafeWrench::UnsafeWrench() : Wrench()
{
}

UnsafeWrench::UnsafeWrench(const Point& P) : Wrench(P)
{
}

UnsafeWrench::UnsafeWrench(const Point& P, const Eigen::Vector3d& force, const Eigen::Vector3d& torque) : Wrench(P,force,torque)
{
}

UnsafeWrench::UnsafeWrench(const UnsafeWrench& rhs)  : Wrench(rhs)
{
}

UnsafeWrench::UnsafeWrench(const Wrench& rhs) : Wrench(rhs)
{
}

UnsafeWrench UnsafeWrench::operator+(const UnsafeWrench& rhs) const
{
    return UnsafeWrench(P, force + rhs.force, torque + rhs.torque);
}

UnsafeWrench UnsafeWrench::operator-(const UnsafeWrench& rhs) const
{
    return UnsafeWrench(P, force - rhs.force, torque - rhs.torque);
}

UnsafeWrench& UnsafeWrench::operator+=(const UnsafeWrench& rhs)
{
    force += rhs.force;
    torque += rhs.torque;
    return *this;
}

UnsafeWrench& UnsafeWrench::operator-=(const UnsafeWrench& rhs)
{
    force -= rhs.force;
    torque -= rhs.torque;
    return *this;
}

UnsafeWrench& UnsafeWrench::operator+=(const Wrench& rhs)
{
    force += rhs.force;
    torque += rhs.torque;
    return *this;
}

UnsafeWrench& UnsafeWrench::operator-=(const Wrench& rhs)
{
    force -= rhs.force;
    torque -= rhs.torque;
    return *this;
}
