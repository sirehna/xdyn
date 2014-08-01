/*
 * DiscreteDirectionalWaveSpectrum.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "WaveDirectionalSpreading.hpp"
#include "WaveSpectralDensity.hpp"

DiscreteDirectionalWaveSpectrum::DiscreteDirectionalWaveSpectrum(const WaveSpectralDensity& S_, const WaveDirectionalSpreading& D_, const double omega_min, const double omega_max, const size_t nfreq) :
          S(S_.clone()),
          D(D_.clone()),
          omega(S->get_angular_frequencies(omega_min, omega_max, nfreq)),
          psi(D->get_directions(nfreq)), domega(1), dpsi(1)
{
    if (omega.size()>1) domega = omega[1]-omega[0];
    if (psi.size()>1)   dpsi = psi[1]-psi[0];

}

DiscreteDirectionalWaveSpectrum::Output::Output() : Si(std::vector<double>()),
                                            Dj(std::vector<double>()),
                                            dw(0),
                                            dpsi(0)
{
}

DiscreteDirectionalWaveSpectrum::Output DiscreteDirectionalWaveSpectrum::get_spectrum() const
{
    Output out;
    BOOST_FOREACH(double w, omega) out.Si.push_back(S->operator ()(w));
    BOOST_FOREACH(double p, psi)   out.Dj.push_back(D->operator ()(p));
    return out;
}

std::vector<double> DiscreteDirectionalWaveSpectrum::get_angular_frequencies() const
{
    return omega;
}

std::vector<double> DiscreteDirectionalWaveSpectrum::get_directions() const
{
    return psi;
}

double DiscreteDirectionalWaveSpectrum::get_domega() const
{
    return domega;
}

double DiscreteDirectionalWaveSpectrum::get_dpsi() const
{
    return dpsi;
}
