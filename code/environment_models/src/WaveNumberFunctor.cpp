/*
 * WaveNumberFunctor.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "WaveNumberFunctor.hpp"

#include <math.h>


WaveNumberFunctor::WaveNumberFunctor(const double h_,    //!< Water depth (in meters)
                  const double omega_ //!< Angular frequency (in rad/s)
                  ) : h(h_), h2(h*h), omega2(omega_*omega_), g(9.81)
{
}

std::tuple<double, double, double>
WaveNumberFunctor::operator()(const double& k) const
{
    const double gk = g*k;
    const double tkh = tanh(k * h);
    const double f = gk*tkh - omega2;
    const double t3 = tkh * tkh;
    const double df = g * tkh + gk * (1 - t3) * h;
    const double t4 = 1 - t3;
    const double d2f = 2 * g * t4 * h - 2 * gk * tkh * t4 * h2;
    return std::make_tuple(f, df, d2f);
}

