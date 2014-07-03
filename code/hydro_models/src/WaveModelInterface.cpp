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

template <typename PointType> PointType compute_relative_position(const PointType& P, const TR1(shared_ptr)<Kinematics>& k)
{
    kinematics::Transform T = k->get("NED", P.get_frame());
    // Create the equivalent transformation just by swapping frame names
    T.swap();
    return T*P;
}

WaveModelInterface::WaveModelInterface()
{
}

WaveModelInterface::~WaveModelInterface()
{
}

double WaveModelInterface::get_relative_wave_height(const Point& P, const TR1(shared_ptr)<Kinematics>& k) const
{
    const Point OP = compute_relative_position(P, k);
    return wave_height(OP.x(),OP.y(),OP.z());
}

std::vector<double> WaveModelInterface::get_relative_wave_height(const PointMatrix& P, const TR1(shared_ptr)<Kinematics>& k) const
{
    const PointMatrix OP = compute_relative_position(P, k);
    const int n = (int)P.m.cols();
    std::vector<double> ret;
    for (int i = 0 ; i < n ; ++i)
    {
        ret.push_back(wave_height(OP.m(0,i),OP.m(1,i),OP.m(2,i)));
    }
    return ret;
}
