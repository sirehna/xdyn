/*
 * DiracDirectionalSpreading.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DIRACDIRECTIONALSPREADING_HPP_
#define DIRACDIRECTIONALSPREADING_HPP_

#include "WaveDirectionalSpreading.hpp"

/** \author cec
 *  \date Jul 31, 2014, 12:20:52 PM
 *  \brief No directional spreading: 1 in a particular direction theta0 & zero elsewhere?
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/DiracDirectionalSpreadingTest.cpp DiracDirectionalSpreadingTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/DiracDirectionalSpreadingTest.cpp DiracDirectionalSpreadingTest expected output
 */
class DiracDirectionalSpreading : public WaveDirectionalSpreading
{
    public:
        DiracDirectionalSpreading(const double psi0 //!< Primary wave direction (NED, "coming from") in radians
                                 );

        /**  \author cec
          *  \date Jul 31, 2014, 12:20:52 PM
          *  \brief Wave density by direction.
          *  \details Equals one if psi=psi0 & 0 otherwise.
          *  \returns Density (between 0 and 1, no unit)
          */
        double operator()(const double psi //!< Primary wave direction in radians.
                         ) const;

    private:
        DiracDirectionalSpreading(); // Disabled
};

#endif /* DIRACDIRECTIONALSPREADING_HPP_ */
