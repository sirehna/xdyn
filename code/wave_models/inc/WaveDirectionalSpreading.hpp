/*
 * WaveDirectionalSpreading.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef WAVEDIRECTIONALSPREADING_HPP_
#define WAVEDIRECTIONALSPREADING_HPP_

/** \author cec
 *  \date Jul 31, 2014, 11:59:52 AM
 *  \brief Distribution of the wave height by direction.
 *  \details Used with WaveSpectralDensity to construct a DirectionalWaveSpectrum
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/WaveDirectionalSpreadingTest.cpp WaveDirectionalSpreadingTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/WaveDirectionalSpreadingTest.cpp WaveDirectionalSpreadingTest expected output
 *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 51
 *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 46
 *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, page 131
 *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 113
 */
class WaveDirectionalSpreading
{
    public:
        WaveDirectionalSpreading(const double psi0 //!< Primary wave direction (NED, "coming from") in radians
                                );
        virtual ~WaveDirectionalSpreading();

        /**  \author cec
          *  \date Jul 31, 2014, 12:03:19 PM
          *  \brief Wave density by direction.
          *  \details Its integral should evaluate to one (because it is a probability density)
          *           The angle convention is "coming from" & the angles are expressed in the
          *           NED frame: 0° corresponds to waves coming from the North & 90° to waves
          *           coming from the East.
          *  \returns Density (between 0 and 1, no unit)
          *  \snippet wave_models/unit_tests/src/WaveDirectionalSpreadingTest.cpp WaveDirectionalSpreadingTest method_example
          */
        virtual double operator()(const double psi //!< Primary wave direction in radians.
                                 ) const = 0;

    protected:
        double psi0; //!< Primary wave direction (NED, "coming from") in radians

    private:
        WaveDirectionalSpreading();
};

#endif /* WAVEDIRECTIONALSPREADING_HPP_ */
