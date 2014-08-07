/*
 * WaveSpectrum.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef WAVESPECTRALDENSITY_HPP_
#define WAVESPECTRALDENSITY_HPP_

#include <vector>
#include <cstdlib>

/** \brief Interface for wave spectral densities (distribution of the wave height by frequency).
 *  \details Used with WaveDirectionalSpreading to construct a DirectionalWaveSpectrum
 *  \addtogroup wave_models
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/WaveSpectrumTest.cpp WaveSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/WaveSpectrumTest.cpp WaveSpectrumTest expected output
 */
class WaveSpectralDensity
{
    public:
        WaveSpectralDensity();
        virtual ~WaveSpectralDensity();
        /**  \brief Computes the amplitude of the power spectrum at a given angular frequency
          *  \returns Amplitude of the power spectrum (in m^2 s)
          *  \snippet wave_models/unit_tests/src/WaveSpectrumTest.cpp WaveSpectrumTest get_example
          */
        virtual double operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                          ) const= 0;

        virtual WaveSpectralDensity* clone() const = 0;

        /**  \brief Returns n angular frequencies between omega_min (included)
          *         and omega_max (also included)
          *  \snippet wave_models/unit_tests/src/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_omega0_example
          */
        virtual std::vector<double> get_angular_frequencies(const double omega_min, //!< Minimum angular frequency (in rad/s)
                                                            const double omega_max, //!< Minimum angular frequency (in rad/s)
                                                            const size_t n          //!< Number of angular frequencies to return
                                                            ) const;

        /**  \brief Compute wave number, in infinite depth.
          *  \returns \f$\frac{omega^2}{g}\f$
          *  \snippet wave_models/unit_tests/src/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_wave_number_example
          *  *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 51
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, equation 3.22, page 67
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.20, page 107
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          */
        double get_wave_number(const double omega //!< Angular frequency (in radians)
                             ) const;

        /**  \brief Compute wave number, in finite depth.
          *  \returns Solution of \f$omega^2=g\cdot k\cdot \tanh{kh}$\f
          *  \snippet wave_models/unit_tests/src/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_wave_number_example
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 51
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, equation 3.20, page 67
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.18, page 107
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          */
        double get_wave_number(const double omega, //!< Angular frequency (in radians)
                               const double h      //!< Depth (in meters)
                             ) const;
};

#endif /* WAVESPECTRALDENSITY_HPP_ */
