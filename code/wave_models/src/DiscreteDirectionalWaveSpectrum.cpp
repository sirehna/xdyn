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

DiscreteDirectionalWaveSpectrum::DiscreteDirectionalWaveSpectrum(const WaveSpectralDensity& S, const WaveDirectionalSpreading& D, const double omega_min, const double omega_max, const size_t nfreq) :
                    Si(std::vector<double>()),
                    Dj(std::vector<double>()),
                    omega(S.get_angular_frequencies(omega_min, omega_max, nfreq)),
                    psi(D.get_directions(nfreq)),
                    domega(1),
                    dpsi(1)
{
    if (omega.size()>1) domega = omega[1]-omega[0];
    if (psi.size()>1)   dpsi = psi[1]-psi[0];
    BOOST_FOREACH(double w, omega) Si.push_back(S(w));
    BOOST_FOREACH(double p, psi)   Dj.push_back(D(p));
}

