/*
 * BretschneiderSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include <cmath>

#include "BretschneiderSpectrum.hpp"

#define POW2(x) ((x)*(x))
#define POW4(x) (POW2(POW2(x)))

BretschneiderSpectrum::BretschneiderSpectrum(const double Hs_,   //!< Significant wave height (in meters)
                                             const double Tp_    //!< Mean wave period (in seconds)
                                             ) : Hs2(Hs_*Hs_), Tp(Tp_), omega0(2.*acos(-1.)/Tp)
{

}

double BretschneiderSpectrum::operator()(const double omega) const
{
    const double alpha = POW4(omega0/omega);
    return 0.3125 * (alpha/omega) * Hs2 * exp(-1.25*alpha);
}

