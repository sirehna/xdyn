/*
 * WaveSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "WaveSpectralDensity.hpp"

WaveSpectralDensity::WaveSpectralDensity()
{

}

WaveSpectralDensity::~WaveSpectralDensity()
{
}

std::vector<double> WaveSpectralDensity::get_angular_frequencies(const double omega_min, //!< Minimum angular frequency (in rad/s)
                                                                 const double omega_max, //!< Minimum angular frequency (in rad/s)
                                                                 const size_t n          //!< Number of angular frequencies to return
                                                                ) const
{
    std::vector<double> omega(n, 0);
    const double Domega = omega_max - omega_min;
    for (size_t i = 1 ; i <= n ; ++i)
    {
        omega[i-1] = omega_min + double(i-1)/double(n-1)*Domega;
    }
    return omega;
}

double WaveSpectralDensity::get_wave_number(const double omega //!< Angular frequency (in radians)
                                           ) const
{
    return omega*omega/9.81;
}
