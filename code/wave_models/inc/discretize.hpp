/*
 * discretize.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef DISCRETIZE_HPP_
#define DISCRETIZE_HPP_

#include "DiscreteDirectionalWaveSpectrum.hpp"

/**  \brief
  *  \details
  *  \returns
  *  \snippet wave_models/unit_tests/src/discretizeTest.cpp discretizeTest flatten_example
  */
FlatDiscreteDirectionalWaveSpectrum flatten(const DiscreteDirectionalWaveSpectrum& spectrum, //!< Spectrum to flatten
                                            const double how_much_energy //!< Between 0 & 1: where should we cut off the spectra?
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

#endif /* DISCRETIZE_HPP_ */
