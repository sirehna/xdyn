/*
 * BretschneiderSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "BretschneiderSpectrum.hpp"
#include "InvalidInputException.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define POW2(x) ((x)*(x))
#define POW4(x) (POW2(POW2(x)))

BretschneiderSpectrum::BretschneiderSpectrum(const double Hs_,   //!< Significant wave height (in meters)
                                             const double Tp_    //!< Mean wave period (in seconds)
                                             ) : Hs2(Hs_*Hs_), Tp(Tp_), omega0(2.*PI)
{
    if (Hs_<0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Hs should be greater than or equal to 0: got " << Hs_);
    }
    if (Tp_<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Tp should be greater than 0: got " << Tp_);
    }
    omega0 /= Tp;
}

double BretschneiderSpectrum::operator()(const double omega) const
{
    if (omega<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega should be greater than 0: got " << omega);
    }
    const double alpha = POW4(omega0/omega);
    return 0.3125 * (alpha/omega) * Hs2 * exp(-1.25*alpha);
}


WaveSpectralDensity* BretschneiderSpectrum::clone() const
{
    return new BretschneiderSpectrum(*this);
}
