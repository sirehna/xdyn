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

Eigen::Vector3d transport(const Eigen::Vector3d& Ma, const Point& a, const Point& b, const Eigen::Vector3d& f);
Eigen::Vector3d transport(const Eigen::Vector3d& Ma, const Point& a, const Point& b, const Eigen::Vector3d& f)
{
	return Ma + (a-b).cross(f);
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

