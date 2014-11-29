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
        DampingMatrixInterpolator();
        std::function<double(double)> build_interpolator(const std::vector<double>& x, const std::vector<double>& y, const TypeOfInterpolation& type) const;
};

#endif /* DAMPINGMATRIXINTERPOLATOR_HPP_ */
