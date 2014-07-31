/*
 * DiracSpectralDensity.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include <cmath>

#include "DiracSpectralDensity.hpp"

DiracSpectralDensity::DiracSpectralDensity(const double omega0_, const double Hs_) : omega0(omega0_), Hs(Hs_)
{
}

double DiracSpectralDensity::operator()(const double omega) const
{
    if (fabs(omega-omega0)<1E-15) return Hs;
                                  return 0;
}

std::vector<double> DiracSpectralDensity::get_angular_frequencies(const double,
                                                                  const double,
                                                                  const size_t
                                                                  ) const
{
    return std::vector<double>(1, omega0);
}

WaveSpectralDensity* DiracSpectralDensity::clone() const
{
    return new DiracSpectralDensity(*this);
}
