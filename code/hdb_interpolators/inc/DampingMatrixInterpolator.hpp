/*
 * DampingMatrixInterpolator.hpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#ifndef DAMPINGMATRIXINTERPOLATOR_HPP_
#define DAMPINGMATRIXINTERPOLATOR_HPP_


#include <functional>

#include "YamlRadiationDamping.hpp"

/** \brief Creates interpolation functions for the damping matrix
 *  \details Used to create the retardation functions
 *  \addtogroup hdb_interpolators
 *  \ingroup hdb_interpolators
 *  \section ex1 Example
 *  \snippet hdb_interpolators/unit_tests/src/DampingMatrixInterpolatorTest.cpp DampingMatrixInterpolatorTest example
 *  \section ex2 Expected output
 *  \snippet hdb_interpolators/unit_tests/src/DampingMatrixInterpolatorTest.cpp DampingMatrixInterpolatorTest expected output
 */
class DampingMatrixInterpolator
{
    public:
        DampingMatrixInterpolator(const TypeOfInterpolation& type_of_interpolation, const TypeOfQuadrature& type_of_quadrature);
        std::function<double(double)> build_interpolator(const std::vector<double>& x, const std::vector<double>& y) const;
        std::function<double(double)> make_retardation_function(const std::function<double(double)>& Br, const double omega_min, const double omega_max, const size_t n) const;

    private:
        DampingMatrixInterpolator();
        double integrate(const std::function<double(double)>& Br, const double tau, const double omega_min, const double omega_max) const;

        TypeOfInterpolation type_of_interpolation;
        TypeOfQuadrature type_of_quadrature;
};

#endif /* DAMPINGMATRIXINTERPOLATOR_HPP_ */
