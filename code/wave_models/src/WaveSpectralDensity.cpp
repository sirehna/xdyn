/*
 * WaveSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */
#include <sstream>

#include "WaveSpectralDensity.hpp"
#include "WaveNumberFunctor.hpp"
#include "WaveModelException.hpp"

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
    if (omega_min<=0)
    {
        std::stringstream ss;
        ss << "omega_min = " << omega_min << ": should be positive.";
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    if (omega_max<=0)
    {
        std::stringstream ss;
        ss << "omega_max = " << omega_max << ": should be positive.";
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    if (omega_max<omega_min)
    {
        std::stringstream ss;
        ss << "omega_max (=" << omega_max << ") < omega_min (=" << omega_min;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    if (omega_max<omega_min)
    {
        std::stringstream ss;
        ss << "omega_max (=" << omega_max << ") < omega_min (=" << omega_min;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    if (n==0)
    {
        std::stringstream ss;
        THROW(__PRETTY_FUNCTION__, WaveModelException, "nfreq == 0");
    }
    if (n==1)
    {
        if (omega_min != omega_max)
        {
            std::stringstream ss;
            ss << "Asked for a single frequency (nfreq = 1), but omega_min (=" << omega_min
               << ") != omega_max (=" << omega_max << ")";
            THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
        }
    }
    if (omega_min==omega_max)
    {
        if (n != 1)
        {
            std::stringstream ss;
            ss << "omega_min = omega_max (=" << omega_min
               << ") but nfreq != 1";
            THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
        }
    }
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

double WaveSpectralDensity::get_wave_number(const double omega, //!< Angular frequency (in radians)
                                            const double h      //!< Depth (in meters)
                                           ) const
{
    WaveNumberFunctor f(h, omega);
    const double guess = get_wave_number(omega); // Tried Guo's formula (2002) as initial guess, but it slows things down by almost 100%
    const double min = guess;
    const double max = 3.5*guess;
    int digits = std::numeric_limits<double>::digits / 2;
    return boost::math::tools::halley_iterate(f, guess, min, max, digits);
}
