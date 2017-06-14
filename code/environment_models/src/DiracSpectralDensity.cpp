/*
 * DiracSpectralDensity.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiracSpectralDensity.hpp"

#include "InvalidInputException.hpp"

#include <cmath>
#include <sstream>


DiracSpectralDensity::DiracSpectralDensity(const double omega0_, const double Hs_) : omega0(omega0_), Hs(Hs_)
{
    if (Hs<0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Hs should be greater than or equal to 0: got " << Hs);
    }
    if (omega0<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega0 should be greater than 0: got " << omega0);
    }

    if (std::isinf(omega0))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega0 is infinite");
    }
}

double DiracSpectralDensity::operator()(const double omega) const
{
    if (omega<=0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "omega should be greater than 0: got " << omega);
    }
    if (fabs(omega-omega0)<1E-15) return Hs*Hs/8;
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
