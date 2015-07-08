/*
 * SumOfWaveSpectralDensities.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: cady
 */

#include <list>
#include <set>

#include "SumOfWaveSpectralDensities.hpp"

SumOfWaveSpectralDensities::SumOfWaveSpectralDensities() : terms(std::vector<TR1(shared_ptr)<WaveSpectralDensity> >())
{
}

SumOfWaveSpectralDensities::SumOfWaveSpectralDensities(const WaveSpectralDensity& w) : terms(std::vector<TR1(shared_ptr)<WaveSpectralDensity> >(1, TR1(shared_ptr)<WaveSpectralDensity>(w.clone())))
{
}

SumOfWaveSpectralDensities::SumOfWaveSpectralDensities(const WaveSpectralDensity& w1, const WaveSpectralDensity& w2) : terms(std::vector<TR1(shared_ptr)<WaveSpectralDensity> >())
{
    terms.push_back(TR1(shared_ptr)<WaveSpectralDensity>(w1.clone()));
    terms.push_back(TR1(shared_ptr)<WaveSpectralDensity>(w2.clone()));
}

SumOfWaveSpectralDensities::SumOfWaveSpectralDensities(const std::vector<WaveSpectralDensity>& ws) : terms(std::vector<TR1(shared_ptr)<WaveSpectralDensity> >())
{
    for (auto t = ws.begin() ; t != ws.end() ; ++t)
    {
        terms.push_back(TR1(shared_ptr)<WaveSpectralDensity>(t->clone()));
    }
}


double SumOfWaveSpectralDensities::operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                                  ) const
{
    double ret = 0;
    for (auto t = terms.begin() ; t != terms.end() ; ++t)
    {
        ret += (*t)->operator()(omega);
    }
    return ret;
}

WaveSpectralDensity* SumOfWaveSpectralDensities::clone() const
{
    return new SumOfWaveSpectralDensities(*this);
}

/**  \brief Returns n angular frequencies between omega_min (included)
  *         and omega_max (also included)
  *  \snippet environment_models/unit_tests/src/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_omega0_example
  */
std::vector<double> SumOfWaveSpectralDensities::get_angular_frequencies(const double omega_min, //!< Minimum angular frequency (in rad/s)
                                            const double omega_max, //!< Minimum angular frequency (in rad/s)
                                            const size_t n          //!< Number of angular frequencies to return
                                            ) const
{
    std::set<double> S;
    for (auto t = terms.begin() ; t != terms.end() ; ++t)
    {
        std::vector<double> f = (*t)->get_angular_frequencies(omega_min, omega_max, n);
        S.insert(f.begin(), f.end());
    }
    std::list<double> L(S.begin(), S.end());
    L.sort();
    return std::vector<double>(L.begin(), L.end());
}
