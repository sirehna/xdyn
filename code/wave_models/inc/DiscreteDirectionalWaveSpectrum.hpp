/*
 * DiscreteDirectionalWaveSpectrum.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DISCRETEDIRECTIONALWAVESPECTRUM_HPP_
#define DISCRETEDIRECTIONALWAVESPECTRUM_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)

#include <vector>

class WaveSpectralDensity;
class WaveDirectionalSpreading;

/** \author cec
 *  \date Jul 31, 2014, 1:08:15 PM
 *  \brief Used by the wave models (eg. Airy, Stokes, etc.)
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/DirectionalWaveSpectrumTest.cpp DirectionalWaveSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/DirectionalWaveSpectrumTest.cpp DirectionalWaveSpectrumTest expected output
 */
struct DiscreteDirectionalWaveSpectrum
{
    DiscreteDirectionalWaveSpectrum();

    std::vector<double> Si;                  //!< Discretized spectral density (in s m^2/rad)
    std::vector<double> Dj;                  //!< Spatial spreading (in 1/rad)
    std::vector<double> omega;               //!< Angular frequencies the spectrum was discretized at (in rad/s)
    std::vector<double> psi;                 //!< Directions between 0 & 2pi the spatial spreading was discretized at (in rad)
    std::vector<double> k;                   //!< Discretized wave number (for each frequency) in rad/m
    std::vector<std::vector<double> > phase; //!< Random phases, for each (direction,frequency) couple (but time invariant) in radian
    double domega;                           //!< Angular frequency discretization step (in rad/s)
    double dpsi;                             //!< Spatial direction discretization step (in rad)
};

#endif /* DISCRETEDIRECTIONALWAVESPECTRUM_HPP_ */
