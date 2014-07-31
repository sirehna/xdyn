/*
 * DiracDirectionalSpreading.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include <cmath>

#include "DiracDirectionalSpreading.hpp"

DiracDirectionalSpreading::DiracDirectionalSpreading(const double psi0_) : WaveDirectionalSpreading(psi0_)
{
}

double DiracDirectionalSpreading::operator()(const double psi) const
{
    if (fabs(psi-psi0)<1E-15) return 1;
                              return 0;
}
