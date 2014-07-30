/*
 * JonswapSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include <cmath>

#include "JonswapSpectrum.hpp"

#define POW2(x) ((x)*(x))
#define POW4(x) (POW2(POW2(x)))

JonswapSpectrum::JonswapSpectrum(const double Hs,const double Tp_,const double gamma_) :
                                   Hs2(Hs*Hs), Tp(Tp_),gamma(gamma_),
                                   omega0(2.*acos(-1.)/Tp), sigma_a(0.07),
                                   sigma_b(0.09), coeff(1-0.287*log(gamma))
{
}

double JonswapSpectrum::operator()(const double omega) const
{
    const double sigma=( omega<=omega0 )?sigma_a:sigma_b;
    const double ratio = omega0/omega;
    const double alpha = POW4(ratio);
    const double Awm5 = 5.0/16.0*alpha/omega*Hs2;
    const double Bwm4 = 1.25*alpha;
    const double r = exp(-0.5*POW2((omega-omega0)/(sigma*omega0)));
    return coeff*Awm5*exp(-Bwm4)*pow(gamma,r);
}

