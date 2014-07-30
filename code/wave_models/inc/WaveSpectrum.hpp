/*
 * WaveSpectrum.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef WAVESPECTRUM_HPP_
#define WAVESPECTRUM_HPP_

/** \author cec
 *  \date Jul 30, 2014, 9:39:20 AM
 *  \brief Interface for wave spectra.
 *  \details Defines what a wave spectrum should do.
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/WaveSpectrumTest.cpp WaveSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/WaveSpectrumTest.cpp WaveSpectrumTest expected output
 */
class WaveSpectrum
{
    public:
        WaveSpectrum();
        virtual ~WaveSpectrum();
        /**  \author cec
          *  \date Jul 30, 2014, 9:40:33 AM
          *  \brief Computes the amplitude of the power spectrum at a given angular frequency
          *  \returns Amplitude of the power spectrum (in m^2 s)
          *  \snippet wave_models/unit_tests/src/WaveSpectrumTest.cpp WaveSpectrumTest get_example
          */
        virtual double operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                          ) const= 0;
};

#endif /* WAVESPECTRUM_HPP_ */
