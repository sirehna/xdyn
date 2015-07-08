/*
 * WaveDirectionalSpreading.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef WAVEDIRECTIONALSPREADING_HPP_
#define WAVEDIRECTIONALSPREADING_HPP_

#include <cstdlib>
#include <vector>

class SumOfWaveDirectionalSpreadings;

/** \author cec
 *  \date Jul 31, 2014, 11:59:52 AM
 *  \brief Distribution of the wave height by direction.
 *  \details Used with WaveSpectralDensity to construct a DirectionalWaveSpectrum
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet environment_models/unit_tests/src/WaveDirectionalSpreadingTest.cpp WaveDirectionalSpreadingTest example
 *  \section ex2 Expected output
 *  \snippet environment_models/unit_tests/src/WaveDirectionalSpreadingTest.cpp WaveDirectionalSpreadingTest expected output
 *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 51
 *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 46
 *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, page 131
 *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 113
 */
class WaveDirectionalSpreading
{
    public:
        WaveDirectionalSpreading();
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
          *  \snippet environment_models/unit_tests/src/WaveDirectionalSpreadingTest.cpp WaveDirectionalSpreadingTest method_example
          */
        virtual double operator()(const double psi //!< Primary wave direction in radians.
                                 ) const = 0;

        virtual WaveDirectionalSpreading* clone() const = 0;

        /**  \author cec
          *  \date Jul 31, 2014, 12:03:19 PM
          *  \details Returns n directions between 0 (included) and 2 pi (excluded)
          *  \snippet environment_models/unit_tests/src/WaveSpectralDensityTest.cpp WaveSpectralDensityTest get_omega0_example
          */
        virtual std::vector<double> get_directions(const size_t n          //!< Number of angles to return
                                                   ) const;

        SumOfWaveDirectionalSpreadings operator+(const WaveDirectionalSpreading& w) const;

    protected:
        double psi0; //!< Primary wave direction (NED, "coming from") in radians
};

#endif /* WAVEDIRECTIONALSPREADING_HPP_ */
