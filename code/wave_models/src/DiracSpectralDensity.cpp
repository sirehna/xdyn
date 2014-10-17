/*
 * DiracSpectralDensity.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include <cmath>
#include <sstream>

#include "DiracSpectralDensity.hpp"
#include "WaveModelException.hpp"

DiracSpectralDensity::DiracSpectralDensity(const double omega0_, const double Hs_) : omega0(omega0_), Hs(Hs_)
{
    if (Hs<0)
    {
        std::stringstream ss;
        ss << "Hs should be greater than or equal to 0: got " << Hs;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
    if (omega0<=0)
    {
        std::stringstream ss;
        ss << "omega0 should be greater than 0: got " << omega0;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
    }
}

double DiracSpectralDensity::operator()(const double omega) const
{
    if (omega<=0)
    {
        std::stringstream ss;
        ss << "omega should be greater than 0: got " << omega;
        THROW(__PRETTY_FUNCTION__, WaveModelException, ss.str());
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
