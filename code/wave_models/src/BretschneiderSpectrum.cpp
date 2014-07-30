/*
 * BretschneiderSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include <cmath>

#include "BretschneiderSpectrum.hpp"

#define WSPI acos(-1.0)
#define WSPOW2(x) ((x)*(x))
#define WSPOW4(x) (WSPOW2(WSPOW2(x)))

BretschneiderSpectrum::BretschneiderSpectrum(const double Hs_,   //!< Significant wave height (in meters)
                                             const double Tp_    //!< Mean wave period (in seconds)
                        ) : Hs(Hs_), Tp(Tp_)
{

}

double BretschneiderSpectrum::operator()(const double W) const
{
    const double W0 = 2.0*WSPI/Tp;
    const double alpha = WSPOW4(W0/W);
    const double psd = 0.3125 * (alpha/W) * (Hs*Hs) * exp(-1.25*alpha);
    return psd;
}

