/*
 * JonswapSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include <cmath>

#include "JonswapSpectrum.hpp"

#ifndef WSPI
#define WSPI acos(-1.0)
#endif

#ifndef WSPOW2
#define WSPOW2(x) ((x)*(x))
#endif

#ifndef WSPOW3
#define WSPOW3(x) ((x)*(x)*(x))
#endif

#ifndef WSPOW4
#define WSPOW4(x) (WSPOW2(WSPOW2(x)))
#endif

#ifndef WSPOW5
#define WSPOW5(x) (WSPOW4(x)*(x))
#endif

#ifndef WSPOW6
#define WSPOW6(x) (WSPOW3(WSPOW2(x)))
#endif

JonswapSpectrum::JonswapSpectrum(const double Hs_,const double Tp_,const double gamma_) :
                                   Hs(Hs_), Tp(Tp_),gamma(gamma_)
{
}


double JonswapSpectrum::operator()(const double W) const
{
    const double W0=2.0*WSPI/Tp;
    const double sigma_a=0.07;
    const double sigma_b=0.09;
    const double sigma=( W<=W0 )?sigma_a:sigma_b;
    const double ratio = W0/W;
    const double alpha = WSPOW4(ratio);
    const double Awm5 = 5.0/16.0*alpha/W*Hs*Hs;
    const double Bwm4 = 1.25*alpha;
    const double coeff = (1-0.287*log(gamma));
    const double r = exp(-0.5*WSPOW2((W-W0)/(sigma*W0)));
    const double psd = coeff*Awm5*exp(-Bwm4)*pow(gamma,r);
    return psd;
}
