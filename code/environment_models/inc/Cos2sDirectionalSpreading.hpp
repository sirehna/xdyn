/*
 * Cos2sDirectionalSpreading.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef COS2SDIRECTIONALSPREADING_HPP_
#define COS2SDIRECTIONALSPREADING_HPP_

#include "WaveDirectionalSpreading.hpp"

/** \ingroup wave_models
 *  \section ex1 Example
 *  \snippet environment_models/unit_tests/src/Cos2sDirectionalSpreadingTest.cpp Cos2sDirectionalSpreadingTest example
 *  \section ex2 Expected output
 *  \snippet environment_models/unit_tests/src/Cos2sDirectionalSpreadingTest.cpp Cos2sDirectionalSpreadingTest expected output
 *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, page 131
 */
class Cos2sDirectionalSpreading : public WaveDirectionalSpreading
{
    public:
        Cos2sDirectionalSpreading(const double psi0, const double s);

        /**  \brief Wave density by direction.
          *  \returns Density (between 0 and 1, no unit)
          */
        double operator()(const double psi //!< Primary wave direction in radians.
                         ) const;

        WaveDirectionalSpreading* clone() const;


    private:
        double s;
        double Fs;
};

#endif /* COS2SDIRECTIONALSPREADING_HPP_ */
