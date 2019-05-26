/*
 * WaveNumberFunctor.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef WAVENUMBERFUNCTOR_HPP_
#define WAVENUMBERFUNCTOR_HPP_

#include <tuple>

/** \author cec
 *  \date Jul 31, 2014, 4:25:23 PM
 *  \brief This is the functor used to solve the dispersion relation to compute
 *         the wave number
 *  \details Provides the values of the function & its first & second derivatives.
 *           The function is \f$k\mapsto g\cdot k\cdot \tanh{kh} - \omega^2$\f
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet environment_models/unit_tests/src/WaveNumberFunctorTest.cpp WaveNumberFunctorTest example
 *  \section ex2 Expected output
 *  \snippet environment_models/unit_tests/src/WaveNumberFunctorTest.cpp WaveNumberFunctorTest expected output
 */
class WaveNumberFunctor
{
    public:
        WaveNumberFunctor(
            const double h,    //!< Water depth (in meters)
            const double omega //!< Angular frequency (in rad/s)
            );

        std::tuple<double, double, double> operator()(const double& k) const;

    private:
        double h;
        double h2;
        double omega2;
        double g;
};

#endif /* WAVENUMBERFUNCTOR_HPP_ */
