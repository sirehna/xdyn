/*
 * AiryInfiniteDepth.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef AIRY_HPP_
#define AIRY_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>


#include "WaveModel.hpp"

/** \brief First order Stokes wave model
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/AiryTest.cpp AiryTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/AiryTest.cpp AiryTest expected output
 */
class Airy : public WaveModel
{
    public:
        Airy(const DiscreteDirectionalWaveSpectrum& spectrum, const int random_number_generator_seed);

        /**  \brief Surface elevation
          *  \returns Elevation of a point at a given instant, in meters.
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 29
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.20, page 125
          *  \snippet wave_models/unit_tests/src/AiryTest.cpp AiryTest elevation_example
          */
        double elevation(const double x,                                  //!< x-position in the NED frame (in meters)
                         const double y,                                  //!< y-position in the NED frame (in meters)
                         const double t                                   //!< Current time instant (in seconds)
                         ) const;

    private:
        Airy(); // Disabled
        std::vector<std::vector<double> > phase;
        boost::mt19937 rng;
        boost::random::uniform_real_distribution<double> generate_random_phase;
};

#endif /* AIRY_HPP_ */
