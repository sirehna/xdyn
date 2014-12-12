/*
 * YamlRadiationDamping.hpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */

#ifndef YAMLRADIATIONDAMPING_HPP_
#define YAMLRADIATIONDAMPING_HPP_

#include <string>

enum class TypeOfInterpolation {PIECEWISE_CONSTANT, LINEAR, SPLINES};
enum class TypeOfQuadrature {RECTANGLE, TRAPEZOIDAL, SIMPSON, GAUSS_KRONROD, BURCHER, CLENSHAW_CURTIS, FILON};

struct YamlRadiationDamping
{
    YamlRadiationDamping();
    std::string         hdb_filename;
    TypeOfInterpolation interpolation;
    TypeOfQuadrature    quadrature;
    double              quadrature_tolerance;
    size_t              nb_of_points_in_retardation_function;
};

#endif /* YAMLRADIATIONDAMPING_HPP_ */
