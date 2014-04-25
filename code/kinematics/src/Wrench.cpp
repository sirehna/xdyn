/*
 * Wrench.cpp
 *
 *  Created on: 24 avr. 2014
 *      Author: cady
 */

#include "Wrench.hpp"

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
