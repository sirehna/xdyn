/*
 * PiersonMoskowitzSpectrum.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef PIERSONMOSKOWITZSPECTRUM_HPP_
#define PIERSONMOSKOWITZSPECTRUM_HPP_


#include "JonswapSpectrum.hpp"

/** \author cec
 *  \date Jul 30, 2014, 12:46:03 PM
 *  \brief
 *  \details
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet environment_models/unit_tests/src/PiersonMoskowitzSpectrum : public WaveSpectrumTest.cpp PiersonMoskowitzSpectrum : public WaveSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet environment_models/unit_tests/src/PiersonMoskowitzSpectrum : public WaveSpectrumTest.cpp PiersonMoskowitzSpectrum : public WaveSpectrumTest expected output
 *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 49
 *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 43
 */
class PiersonMoskowitzSpectrum : public JonswapSpectrum
{
    public:
        /**  \author cec
          *  \date Jul 30, 2014, 10:04:59 AM
          *  \brief PiersonMoskowitzSpectrum only constructor
          *  \details Default constructor was disabled by making it private & providing no implementation
          */
        PiersonMoskowitzSpectrum(
              const double Hs,   //!< Significant wave height (in meters)
              const double Tp    //!< Mean wave period (in seconds));
              );

        WaveSpectralDensity* clone() const;

    private:
        PiersonMoskowitzSpectrum(); // Disabled
};

#endif /* PIERSONMOSKOWITZSPECTRUM_HPP_ */
