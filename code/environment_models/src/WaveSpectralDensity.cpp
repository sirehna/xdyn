/*
 * WaveSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */
#include <limits>

#include <boost/math/tools/roots.hpp>

#include "SumOfWaveSpectralDensities.hpp"
#include "WaveSpectralDensity.hpp"
#include "WaveNumberFunctor.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include <cmath> // For isnan

WaveSpectralDensity::WaveSpectralDensity()
{
}

SumOfWaveSpectralDensities WaveSpectralDensity::operator+(const WaveSpectralDensity& w) const
{
    return SumOfWaveSpectralDensities(*this, w);
}

WaveSpectralDensity::~WaveSpectralDensity()
{
}

std::vector<double> WaveSpectralDensity::get_angular_frequencies(const double omega_min, //!< Minimum angular frequency (in rad/s)
                                                                 const double omega_max, //!< Minimum angular frequency (in rad/s)
                                                                 const size_t n          //!< Number of angular frequencies to return
                                                                ) const
{
    if (std::isinf(omega_min))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "omega_min is infinite");
    }
    if (std::isinf(omega_max))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "omega_max is infinite");
    }
    if (omega_min<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega_min = " << omega_min << ": should be positive.");
    }
    if (omega_max<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega_max = " << omega_max << ": should be positive.");
    }
    if (omega_max<omega_min)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega_max (=" << omega_max << ") < omega_min (=" << omega_min);
    }
    if (omega_max<omega_min)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega_max (=" << omega_max << ") < omega_min (=" << omega_min);
    }
    if (n==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "nfreq == 0");
    }
    if (n==1)
    {
        if (omega_min != omega_max)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Asked for a single frequency (nfreq = 1), but omega_min (=" << omega_min
                    << ") != omega_max (=" << omega_max << ")");
        }
        return std::vector<double>(1, omega_min);
    }
    if (omega_min==omega_max)
    {
        if (n != 1)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega_min = omega_max (=" << omega_min
                    << ") but nfreq != 1");
        }
        return std::vector<double>(1, omega_min);
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
    if (std::isinf(omega))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Cannot compute wave number for omega = inf");
    }
    if (omega == 0)
    {
        return 0;
    }
    WaveNumberFunctor f(h, omega);
    const double guess = get_wave_number(omega); // Tried Guo's formula (2002) as initial guess, but it slows things down by almost 100%
    const double min = guess;
    const double max = 3.5*guess;
    int digits = std::numeric_limits<double>::digits / 2;
    const double k = boost::math::tools::halley_iterate(f, guess, min, max, digits);
    if (std::isnan(k))
    {
        COUT(omega);
        COUT(guess);
        COUT(min);
        COUT(max);
        COUT(digits);
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Halley iterate for wave number k converged to NaN");
    }
    return k;
}
