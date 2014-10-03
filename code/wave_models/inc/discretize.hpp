/*
 * discretize.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

/** @file */

#ifndef DISCRETIZE_HPP_
#define DISCRETIZE_HPP_

#include "DiscreteDirectionalWaveSpectrum.hpp"

/**  \brief Only select the most important spectrum components & create single vector
  *  \details No need to loop on all frequencies & all directions: we only select
  *  the most important ones (i.e. those representing a given ratio of the total
  *  energy in the spectrum.
  *  \returns A flat spectrum (i.e. one where the freq & direct. loops have been unrolled)
  *  \snippet wave_models/unit_tests/src/discretizeTest.cpp discretizeTest flatten_example
  */
FlatDiscreteDirectionalWaveSpectrum flatten(const DiscreteDirectionalWaveSpectrum& spectrum, //!< Spectrum to flatten
                                            const double energy_ratio //!< Between 0 & 1: where should we cut off the spectra?
                                            );

/**  \author cec
  *  \date Aug 1, 2014, 5:04:24 PM
  *  \brief Discretize a wave spectrum
  *  \details Infinite depth hypothesis (for the wave number)
  *  \snippet wave_models/unit_tests/src/discretizeTest.cpp discretizeTest discretize_example
  */
DiscreteDirectionalWaveSpectrum discretize(const WaveSpectralDensity& S,      //!< Frequency spectrum
                                           const WaveDirectionalSpreading& D, //!< Spatial spectrum
                                           const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
                                           const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
                                           );

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
                                           );

/**  \brief Utility function used by the discretize function. Infinite depth approximation
  *  \returns Factor \f$f(k,z)\f$ such that \f$p_{\mbox{dyn}}=\rho g \eta_a f(k,z)\f$ (no unit), infinite depth approximation
  *  \snippet wave_models/unit_tests/src/discretizeTest.cpp discretizeTest dynamic_pressure_factor example
  */
double dynamic_pressure_factor(const double k, //!< Wave number (in 1/m)
                               const double z  //!< z-position in the NED frame (in meters)
                              );

/**  \brief Utility function used by the discretize function. Finite depth.
  *  \returns Factor \f$f(k,z,h)\f$ such that \f$p_{\mbox{dyn}}=\rho g \eta_a f(k,z,h)\f$ (no unit), finite depth.
  *  \snippet wave_models/unit_tests/src/discretizeTest.cpp discretizeTest dynamic_pressure_factor example
  */
double dynamic_pressure_factor(const double k, //!< Wave number (in 1/m)
                               const double z, //!< z-position in the NED frame (in meters)
                               const double h  //!< Average water depth (in meters)
                              );

#endif /* DISCRETIZE_HPP_ */
