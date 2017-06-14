/*
 * SumOfWaveDirectionalSpreadings.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: cady
 */

#include "SumOfWaveDirectionalSpreadings.hpp"

#include "SumOfWaveSpectralDensities.hpp"

#include <list>
#include <set>


SumOfWaveDirectionalSpreadings::SumOfWaveDirectionalSpreadings(const WaveDirectionalSpreading& w) : terms(std::vector<TR1(shared_ptr)<WaveDirectionalSpreading> >(1, TR1(shared_ptr)<WaveDirectionalSpreading>(w.clone())))
{
}

SumOfWaveDirectionalSpreadings::SumOfWaveDirectionalSpreadings(const WaveDirectionalSpreading& w1, const WaveDirectionalSpreading& w2) : terms(std::vector<TR1(shared_ptr)<WaveDirectionalSpreading> >())
{
    terms.push_back(TR1(shared_ptr)<WaveDirectionalSpreading>(w1.clone()));
    terms.push_back(TR1(shared_ptr)<WaveDirectionalSpreading>(w2.clone()));
}

SumOfWaveDirectionalSpreadings::SumOfWaveDirectionalSpreadings(const std::vector<WaveDirectionalSpreading>& ws) : terms(std::vector<TR1(shared_ptr)<WaveDirectionalSpreading> >())
{
    for (auto t = ws.begin() ; t != ws.end() ; ++t)
    {
        terms.push_back(TR1(shared_ptr)<WaveDirectionalSpreading>(t->clone()));
    }
}


double SumOfWaveDirectionalSpreadings::operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                                  ) const
{
    double ret = 0;
    for (auto t = terms.begin() ; t != terms.end() ; ++t)
    {
        ret += (*t)->operator()(omega);
    }
    return ret;
}

WaveDirectionalSpreading* SumOfWaveDirectionalSpreadings::clone() const
{
    return new SumOfWaveDirectionalSpreadings(*this);
}

/**  \brief Returns n angular frequencies between omega_min (included)
  *         and omega_max (also included)
  *  \snippet environment_models/unit_tests/src/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_omega0_example
  */
std::vector<double> SumOfWaveDirectionalSpreadings::get_directions(const size_t n) const
{
    std::set<double> S;
    for (auto t = terms.begin() ; t != terms.end() ; ++t)
    {
        std::vector<double> f = (*t)->get_directions(n);
        S.insert(f.begin(), f.end());
    }
    std::list<double> L(S.begin(), S.end());
    L.sort();
    return std::vector<double>(L.begin(), L.end());
}


