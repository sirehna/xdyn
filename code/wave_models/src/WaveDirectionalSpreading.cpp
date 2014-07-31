/*
 * WaveDirectionalSpreading.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "WaveDirectionalSpreading.hpp"

WaveDirectionalSpreading::WaveDirectionalSpreading(const double psi0_) : psi0(psi0_)
{
}

WaveDirectionalSpreading::~WaveDirectionalSpreading()
{
}

std::vector<double> WaveDirectionalSpreading::get_directions(const size_t n          //!< Number of angles to return
                                                   ) const
{
    std::vector<double> psi(n, 0);
    const double two_pi = 2*PI;
    for (size_t i = 0 ; i < n ; ++i)
    {
        psi[i] = double(i)/double(n)*two_pi;
    }
    return psi;
}
