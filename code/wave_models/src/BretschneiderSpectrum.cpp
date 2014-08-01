/*
 * BretschneiderSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <sstream>

#include "BretschneiderSpectrum.hpp"
#include "WaveModelException.hpp"

#define POW2(x) ((x)*(x))
#define POW4(x) (POW2(POW2(x)))

BretschneiderSpectrum::BretschneiderSpectrum(const double Hs_,   //!< Significant wave height (in meters)
                                             const double Tp_    //!< Mean wave period (in seconds)
                                             ) : Hs2(Hs_*Hs_), Tp(Tp_), omega0(2.*PI)
{
    if (Hs_<0)
    {
        std::stringstream ss;
        ss << "Hs should be greater than or equal to 0: got " << Hs_;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    if (Tp_<=0)
    {
        std::stringstream ss;
        ss << "Tp should be greater than 0: got " << Tp_;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    omega0 /= Tp;
}

double BretschneiderSpectrum::operator()(const double omega) const
{
    if (omega<=0)
    {
        std::stringstream ss;
        ss << "omega should be greater than 0: got " << omega;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    const double alpha = POW4(omega0/omega);
    return 0.3125 * (alpha/omega) * Hs2 * exp(-1.25*alpha);
}


WaveSpectralDensity* BretschneiderSpectrum::clone() const
{
    return new BretschneiderSpectrum(*this);
}
