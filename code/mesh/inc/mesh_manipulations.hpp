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
  *  \returns A unit vector orthogonal to the plane in which the polygon lies, & oriented by the right-hand rule (if oriented_clockwise==false)
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

/**  \brief Computes the iso-braycenter of a list of points
  *  \returns The iso-barycenter of the points
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest barycenter_example
  */
Eigen::Vector3d barycenter(const VectorOfVectorOfPoints& points //!< List of points
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
  *  \date May 21, 2014, 10:39:36 AM
  *  \brief Calculates the area of a triangle
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest triangle_area_example
  */
double area(const Matrix3x& points, //!< Matrix containing (amongst others), the points of interest
            const int idxA,         //!< Index of the column containing the first point
            const int idxB,         //!< Index of the column containing the second point
            const int idxC          //!< Index of the column containing the third point
            );

/**  \author cec
  *  \date May 20, 2014, 10:55:32 AM
  *  \brief Computes the position of the centre of gravity of a polygon
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest centre_of_gravity_example
  */
Eigen::Vector3d centre_of_gravity(const Matrix3x& polygon //!< Polygon we wish to compute the centre of gravity of
                                 );

/**  \brief Writes a mesh to output stream, in binary STL format
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest read_binary_stl_example
  */
void write_binary_stl(const VectorOfVectorOfPoints& stl, std::ostream& os);

/**  \brief Convert a VectorOfPoints to an Eigen::Matrix3s
  *  \details Each line corresponds to a coordinate & each column to a point.
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest convert_example
  */
Matrix3x convert(const VectorOfPoints& v //!< Points to convert
                );

/**  \brief Determines if the points in the facet are oriented by the right-hand rule
  *  \returns True if the points in the mesh are oriented by the right-hand rule, false otherwise
  */
bool oriented_clockwise(const VectorOfPoints& v, //!< Points in mesh
                        const EPoint& O //!< Point inside the volume (eg. its centre of gravity)
        );

/**  \brief Determines for each facet if the points in the mesh are oriented by the right-hand rule
  *  \returns True all facets in the mesh are oriented by the right-hand rule (anti-clockwise),
  *           false if all facets are oriented clockwise, throws otherwise.
  */
bool oriented_clockwise(const VectorOfVectorOfPoints& v, //!< Points in mesh
                        const EPoint& O //!< Point inside the volume (eg. its centre of gravity)
        );

#endif /* MESH_MANIPULATIONS_HPP_ */
