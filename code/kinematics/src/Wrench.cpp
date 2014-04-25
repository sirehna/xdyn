/*
 * Wrench.cpp
 *
 *  Created on: 24 avr. 2014
 *      Author: cady
 */

#include "Wrench.hpp"
#include "KinematicsException.hpp"
#include "almost_equal.hpp"

bool equal(const Point& P, const Point& Q);
bool equal(const Point& P, const Point& Q)
{
	if (P.get_frame() != Q.get_frame()) return false;
	if (not(almost_equal(P.x, Q.x)))    return false;
	if (not(almost_equal(P.y, Q.y)))    return false;
	if (not(almost_equal(P.z, Q.z)))    return false;
	                                    return true;
}


Wrench::Wrench(const Point& P_) : force(Eigen::Vector3d()),
		                          torque(Eigen::Vector3d()),
                                  X(force[0]),
                                  Y(force[1]),
                                  Z(force[2]),
                                  K(torque[0]),
                                  M(torque[1]),
                                  N(torque[2]),
                                  P(P_)
{
}

Wrench::Wrench(const Point& P_, const Eigen::Vector3d& force_, const Eigen::Vector3d& torque_) : force(force_),
		                                                                                         torque(torque_),
		                                                                                         X(force[0]),
		                                                                                         Y(force[1]),
		                                                                                         Z(force[2]),
		                                                                                         K(torque[0]),
		                                                                                         M(torque[1]),
		                                                                                         N(torque[2]),
		                                                                                         P(P_)
{
}


Wrench::Wrench(const Wrench& rhs) : force(rhs.force),
		                          torque(rhs.torque),
                                  X(force[0]),
                                  Y(force[1]),
                                  Z(force[2]),
                                  K(torque[0]),
                                  M(torque[1]),
                                  N(torque[2]),
                                  P(rhs.P)
{
}

Wrench& Wrench::operator=(const Wrench& rhs)
{
	if (this != &rhs)
	{
		force = rhs.force;
		torque = rhs.torque;
		X = force[0];
		Y = force[1];
		Z = force[2];
		K = torque[0];
		M = torque[1];
		N = torque[2];
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

