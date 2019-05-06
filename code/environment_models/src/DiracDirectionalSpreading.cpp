/*
 * DiracDirectionalSpreading.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiracDirectionalSpreading.hpp"

#include <cmath>


DiracDirectionalSpreading::DiracDirectionalSpreading(const double psi0_) : WaveDirectionalSpreading(psi0_)
{
}

double DiracDirectionalSpreading::operator()(const double psi) const
{
    if (fabs(psi-psi0)<1E-15)
    {
        return 1.0;
    }
    return 0.0;
}

WaveDirectionalSpreading* DiracDirectionalSpreading::clone() const
{
    return new DiracDirectionalSpreading(*this);
}

std::vector<double> DiracDirectionalSpreading::get_directions(const size_t) const
{
    return std::vector<double>(1, psi0);
}
