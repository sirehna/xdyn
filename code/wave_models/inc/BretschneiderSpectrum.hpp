/*
 * BretschneiderSpectrum.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef BRETSCHNEIDERSPECTRUM_HPP_
#define BRETSCHNEIDERSPECTRUM_HPP_

#include "WaveSpectrum.hpp"

/** \author cec
 *  \date Jul 30, 2014, 2:25:11 PM
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/BretschneiderSpectrumTest.cpp BretschneiderSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/BretschneiderSpectrumTest.cpp BretschneiderSpectrumTest expected output
 */
class BretschneiderSpectrum : public WaveSpectrum
{
    public:
        /**  \author cec
          *  \date Jul 30, 2014, 10:04:59 AM
          *  \brief BretschneiderSpectrum only constructor
          *  \details Default constructor was disabled by making it private & providing no implementation
          */
        BretschneiderSpectrum(const double Hs,   //!< Significant wave height (in meters)
                              const double Tp    //!< Mean wave period (in seconds)
                        );

        /**  \author cec
          *  \date Jul 30, 2014, 9:40:33 AM
          *  \brief Computes the amplitude of the power spectrum at a given angular frequency
          *  \returns Amplitude of the power spectrum (in m^2 s)
          *  \snippet wave_models/unit_tests/src/BretschneiderSpectrumTest.cpp BretschneiderSpectrumTest example
          */
        double operator()(const double omega //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
                         ) const;


    private:
        BretschneiderSpectrum(); // Disabled
        double Hs;
        double Tp;
};

#endif /* BRETSCHNEIDERSPECTRUM_HPP_ */
