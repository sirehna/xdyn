/*
 * WaveModelInterface.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "WaveModelInterface.hpp"
#include "Kinematics.hpp"
#include "Point.hpp"
#include "Transform.hpp"

WaveModelInterface::WaveModelInterface(const std::tr1::shared_ptr<Kinematics>& k_) : k(k_)
{
}

WaveModelInterface::~WaveModelInterface()
{
}

double WaveModelInterface::get_relative_wave_height(const Point& P) const
{
    const kinematics::Transform T = k->get("NED", P.get_frame());
    // O is the origin of the NED frame
    // O1 is the origin of P's frame
    const Point OO1 = T*Point("NED");
    const Eigen::Vector3d OP = OO1 + P;
    return wave_height(OP[0],OP[1],OP[2]);
}
