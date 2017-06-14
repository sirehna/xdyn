/*
 * JonswapSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "JonswapSpectrum.hpp"

#include "InvalidInputException.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define POW2(x) ((x)*(x))
#define POW4(x) (POW2(POW2(x)))

JonswapSpectrum::JonswapSpectrum(const double Hs,const double Tp_,const double gamma_) :
                                   Hs2(Hs*Hs), Tp(Tp_),gamma(gamma_),
                                   omega0(2.*PI), sigma_a(0.07),
                                   sigma_b(0.09), coeff(0)
{
    if (Hs<0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Hs should be greater than or equal to 0: got " << Hs);
    }
    if (Tp_<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Tp should be greater than 0: got " << Tp_);
    }
    if (gamma_<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "gamma should be greater than 0: got " << gamma_);
    }
    omega0 /= Tp;
    coeff = 1-0.287*log(gamma);
}

double JonswapSpectrum::operator()(const double omega) const
{
    if (omega<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega should be greater than 0: got " << omega);
    }
    const double sigma=( omega<=omega0 )?sigma_a:sigma_b;
    const double ratio = omega0/omega;
    const double alpha = POW4(ratio);
    const double Awm5 = 5.0/16.0*alpha/omega*Hs2;
    const double Bwm4 = 1.25*alpha;
    const double r = exp(-0.5*POW2((omega-omega0)/(sigma*omega0)));
    return coeff*Awm5*exp(-Bwm4)*pow(gamma,r);
}

WaveSpectralDensity* JonswapSpectrum::clone() const
{
    return new JonswapSpectrum(*this);
}
