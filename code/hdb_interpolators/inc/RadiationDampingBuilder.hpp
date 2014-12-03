/*
 * RadiationDampingBuilder.hpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#ifndef RADIATIONDAMPINGBUILDER_HPP_
#define RADIATIONDAMPINGBUILDER_HPP_


#include <functional>

#include "YamlRadiationDamping.hpp"

class History;

/** \brief Creates interpolation functions for the damping matrix
 *  \details Used to create the retardation functions
 *  \addtogroup hdb_interpolators
 *  \ingroup hdb_interpolators
 *  \section ex1 Example
 *  \snippet hdb_interpolators/unit_tests/src/DampingMatrixInterpolatorTest.cpp DampingMatrixInterpolatorTest example
 *  \section ex2 Expected output
 *  \snippet hdb_interpolators/unit_tests/src/DampingMatrixInterpolatorTest.cpp DampingMatrixInterpolatorTest expected output
 */
class RadiationDampingBuilder
{
    public:
        RadiationDampingBuilder(const TypeOfInterpolation& type_of_interpolation, //!< Splines, piecewise constant or linear
                                const TypeOfQuadrature& type_of_quadrature //!< Gauss-Kronrod, rectangle, Simpson or trapezoidal
                                );

        /**  \brief Build a continuous function from discrete (x,y) points (using interpolation type defined in constructor)
          */
        std::function<double(double)> build_interpolator(const std::vector<double>& x, const std::vector<double>& y) const;

        /**  \brief Use radiation damping function to compute retardation function
          *  \details Radiation damping can come, eg. from the build_interpolator method. This method evaluates the
          *  integral K(\tau) := \frac{2}{\pi} int_{\omega_{\mbox{min}}}^{\omega_{\mbox{max}}} B_r(\omega)\cos(\omega\cdot \tau)d\tau
          *  for n different values of \tau between \frac{2\pi}{\omega{\mbox{max}}} and \frac{2\pi}{\omega{\mbox{min}}}.
          *  It then uses "build_interpolator" to build a function (lambda).
          */
        std::function<double(double)> build_retardation_function(const std::function<double(double)>& Br, //!< Radiation damping function
                                                                 const double omega_min, //!< Lower bound of the integration
                                                                 const double omega_max, //!< Upper bound of the integration
                                                                 const size_t n //!< Number of times Br is evaluated (number of points used to build interpolator)
                                                                 ) const;
        /**  \brief Computes the convolution of a function with state history, over a certain time
          *  \returns int_0^T h(t-tau)*f(tau) dtau
          *  \snippet hdb_interpolators/unit_tests/src/RadiationDampingBuilderTest.cpp RadiationDampingBuilderTest method_example
          */
        double convolution(const History& h, //!< State history
                           const std::function<double(double)>& f, //!< Function to convolute with
                           const double T //!< Length of the convolution (in seconds)
                           ) const;

    private:
        RadiationDampingBuilder();
        double integrate(const std::function<double(double)>& Br, const double tau, const double omega_min, const double omega_max) const;
        double integrate(const std::function<double(double)>& f, const double a, const double b) const;

        TypeOfInterpolation type_of_interpolation;
        TypeOfQuadrature type_of_quadrature;
};

#endif /* RADIATIONDAMPINGBUILDER_HPP_ */
