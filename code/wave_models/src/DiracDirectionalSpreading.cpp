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

WaveDirectionalSpreading* DiracDirectionalSpreading::clone() const
{
    return new DiracDirectionalSpreading(*this);
}

std::vector<double> DiracDirectionalSpreading::get_directions(const size_t) const
{
    return std::vector<double>(1, psi0);
}
