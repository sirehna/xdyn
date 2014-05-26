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
#include "PointMatrix.hpp"

template <typename PointType> PointType compute_relative_position(const PointType& P, const std::tr1::shared_ptr<Kinematics>& k)
{
    // Transform from NED to P's frame of reference
    const kinematics::Transform T = k->get("NED", P.get_frame());
    // O is the origin of the NED frame
    const Point O("NED");
    // O1 is the origin of P's frame
    const Point OO1 = T*O;
    // Return coordinates of P in the NED frame
    return OO1 + P;
}

WaveModelInterface::WaveModelInterface()
{
}

WaveModelInterface::~WaveModelInterface()
{
}

double WaveModelInterface::get_relative_wave_height(const Point& P, const std::tr1::shared_ptr<Kinematics>& k) const
{
    const Point OP = compute_relative_position(P, k);
    return wave_height(OP.x,OP.y,OP.z);
}

std::vector<double> WaveModelInterface::get_relative_wave_height(const PointMatrix& P, const std::tr1::shared_ptr<Kinematics>& k) const
{
    const PointMatrix OP = compute_relative_position(P, k);
    const size_t n = P.m.cols();
    std::vector<double> ret;
    for (size_t i = 0 ; i < n ; ++i)
    {
        ret.push_back(wave_height(OP.m(0,i),OP.m(1,i),OP.m(2,i)));
    }
    return ret;
}
