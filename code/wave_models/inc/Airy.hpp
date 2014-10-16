/*
 * AiryInfiniteDepth.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef AIRY_HPP_
#define AIRY_HPP_

#include <ssc/macros.hpp>
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
        Airy(const DiscreteDirectionalWaveSpectrum& spectrum, const double constant_random_phase); // For tests
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

        /**  \brief Unsteady pressure field induced by undisturbed waves. Used to compute the Froude-Krylov forces.
          *  \details Also called "subsurface pressure" (by DNV), "unsteady pressure" (by Faltinsen) or constant pressure contour (by Lloyd)
          *           The dynamic pressure is in fact one of the terms of Bernoulli's equation, which can be derived from the conservation
          *           of energy for a fluid in motion.
          *           \f[\int_C \frac{\partial \Phi_I(x,y,z,t)}{\partial t}\f]
          *  \returns Dynamic pressure in Pascal
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation VI.34, page 183
          *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 68
          *  \see "The dynamic of marine craft", 2004, Lewandoski, page 148
          *  \snippet wave_models/unit_tests/src/AiryTest.cpp AiryTest elevation_example
          */
        double dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                const double g,   //!< gravity (in m/s^2)
                                const double x,   //!< x-position in the NED frame (in meters)
                                const double y,   //!< y-position in the NED frame (in meters)
                                const double z,   //!< z-position in the NED frame (in meters)
                                const double eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
                                const double t    //!< Current time instant (in seconds)
                                ) const;



    private:
        Airy(); // Disabled
        std::vector<std::vector<double> > phase;
        boost::mt19937 rng;
        boost::random::uniform_real_distribution<double> generate_random_phase;
};

#endif /* AIRY_HPP_ */
