/*
 * DirectionalWaveSpectrum.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DIRECTIONALWAVESPECTRUM_HPP_
#define DIRECTIONALWAVESPECTRUM_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)

#include <vector>

class WaveSpectralDensity;
class WaveDirectionalSpreading;


/** \author cec
 *  \date Jul 31, 2014, 1:08:15 PM
 *  \brief Used by the wave models (eg. Airy, Stokes, etc.) to
 *  \details
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/DirectionalWaveSpectrumTest.cpp DirectionalWaveSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/DirectionalWaveSpectrumTest.cpp DirectionalWaveSpectrumTest expected output
 */
class DirectionalWaveSpectrum
{
    public:
        DirectionalWaveSpectrum(const WaveSpectralDensity& S, const WaveDirectionalSpreading& D, const double omega_min, const double omega_max, const size_t nfreq);

        struct Output
        {
            Output();
            std::vector<double> Si;
            std::vector<double> Dj;
            double dw;
            double dpsi;
        };

        Output get_spectra() const;

        std::vector<double> get_angular_frequencies() const;
        std::vector<double> get_directions() const;
        double get_domega() const;
        double get_dpsi() const;

    private:
        TR1(shared_ptr)<WaveSpectralDensity> S;
        TR1(shared_ptr)<WaveDirectionalSpreading> D;
        std::vector<double> omega;
        std::vector<double> psi;
        double domega;
        double dpsi;
};

#endif /* DIRECTIONALWAVESPECTRUM_HPP_ */
