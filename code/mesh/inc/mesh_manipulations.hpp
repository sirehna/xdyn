/*
 * mesh_manipulations.hpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#ifndef MESH_MANIPULATIONS_HPP_
#define MESH_MANIPULATIONS_HPP_

#include "GeometricTypes3d.hpp"

/**  \author cec
  *  \date May 20, 2014, 8:47:20 AM
  *  \brief Returns the polygon's unit normal vector
  *  \details Assumes the points are in the same plane & are given in direct order (right-hand rule)
  *  \returns A unit vector orthogonal to the plane in which the polygon lies, & oriented by the right-hand rule
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest unit_normal_example
  */
Eigen::Vector3d unit_normal(const Matrix3x& polygon //!< Polygon for which the unit normal vector is computed
                           );

/**  \author cec
  *  \date May 20, 2014, 8:47:20 AM
  *  \brief Computes the iso-braycenter of a list of points
  *  \returns The iso-barycenter of the points
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest barycenter_example
  */
Eigen::Vector3d barycenter(const Matrix3x& points //!< List of points
                          );

/**  \author cec
  *  \date May 20, 2014, 8:47:20 AM
  *  \brief Computes the area of a polygon.
  *  \details Decomposes the polygon in triangles & sums the areas
  *  \returns The area of the polygon
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest area_example
  */
double area(const Matrix3x& polygon //!< Polygon for which the area is computed
        );

/**  \author cec
  *  \date May 20, 2014, 10:55:32 AM
  *  \brief Computes the position of the centre of gravity of a polygon
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest centre_of_gravity_example
  */
Eigen::Vector3d centre_of_gravity(const Matrix3x& polygon //!< Polygon we wish to compute the centre of gravity of
                                 );


#endif /* MESH_MANIPULATIONS_HPP_ */
