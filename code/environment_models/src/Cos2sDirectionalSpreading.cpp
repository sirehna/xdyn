/*
 * Cos2sDirectionalSpreading.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "Cos2sDirectionalSpreading.hpp"

#include "InvalidInputException.hpp"

#include <boost/math/special_functions/gamma.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI


Cos2sDirectionalSpreading::Cos2sDirectionalSpreading(const double psi0, const double s_) : WaveDirectionalSpreading(psi0),
s(s_),
Fs(0)
{
    if (s<0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "s = " << s << ": should be non-negative.");
    }
    Fs = pow(2,2*s-1.)/PI*pow(boost::math::tgamma(s+1.),2)/boost::math::tgamma(2*s+1.);
}

double Cos2sDirectionalSpreading::operator()(const double psi //!< Primary wave direction in radians.
                                            ) const
{
    return Fs*pow(cos((psi-psi0)/2),2*s);
}

WaveDirectionalSpreading* Cos2sDirectionalSpreading::clone() const
{
    return new Cos2sDirectionalSpreading(*this);
}
