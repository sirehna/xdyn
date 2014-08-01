/*
 * discretize.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "discretize.hpp"
#include "WaveDirectionalSpreading.hpp"
#include "WaveSpectralDensity.hpp"

DiscreteDirectionalWaveSpectrum common(const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
        );
DiscreteDirectionalWaveSpectrum common(const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
        )
{
    DiscreteDirectionalWaveSpectrum ret;
    ret.omega = S.get_angular_frequencies(omega_min, omega_max, nfreq);
    ret.psi = D.get_directions(nfreq);
    if (ret.omega.size()>1) ret.domega = ret.omega[1]-ret.omega[0];
    if (ret.psi.size()>1)   ret.dpsi = ret.psi[1]-ret.psi[0];
    BOOST_FOREACH(double omega, ret.omega) ret.Si.push_back(S(omega));
    BOOST_FOREACH(double psi, ret.psi)   ret.Dj.push_back(D(psi));
    return ret;
}

DiscreteDirectionalWaveSpectrum discretize(const WaveSpectralDensity& S,      //!< Frequency spectrum
                                           const WaveDirectionalSpreading& D, //!< Spatial spectrum
                                           const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
                                           )
{
    DiscreteDirectionalWaveSpectrum ret = common(S,D,omega_min,omega_max,nfreq);
    BOOST_FOREACH(double omega, ret.omega) ret.k.push_back(S.get_wave_number(omega));
    return ret;
}

/**  \author cec
  *  \date Aug 1, 2014, 5:04:24 PM
  *  \brief Discretize a wave spectrum
  *  \details Finite depth hypothesis (for the wave number)
  *  \snippet wave_models/unit_tests/src/discretizeTest.cpp discretizeTest discretize_example
  */
DiscreteDirectionalWaveSpectrum discretize(const WaveSpectralDensity& S,      //!< Frequency spectrum
                                           const WaveDirectionalSpreading& D, //!< Spatial spectrum
                                           const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const size_t nfreq,                //!< Number of frequencies & number of directions in discrete spectrum
                                           const double h                     //!< Water depth (in meters)
                                           )
{
    DiscreteDirectionalWaveSpectrum ret = common(S,D,omega_min,omega_max,nfreq);
    BOOST_FOREACH(double omega, ret.omega) ret.k.push_back(S.get_wave_number(omega,h));
    return ret;
}

