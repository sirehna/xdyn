/*
 * mesh_manipulations.hpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#ifndef MESH_MANIPULATIONS_HPP_
#define MESH_MANIPULATIONS_HPP_

#include "GeometricTypes3d.hpp"
#include "MeshIntersector.hpp"


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
/**
  *  \brief Computes the barycenter of a polygon given by vertex index.
  *  \details Decomposes the polygon in triangles & sums the areas
  */
Eigen::Vector3d barycenter(const Matrix3x& p,std::vector<size_t> &vertex_index);

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

/**
  *  \brief Computes the area of a polygon given by vertex index.
  *  \details Decomposes the polygon in triangles & sums the areas
  */
double area(const Matrix3x& points,std::vector<size_t> &vertex_index);

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


/**  \details Compute the inertia matrix for a triangular facet versus an inertia frame, divided by triangle area;
  *  the return inertia is w.r.t. frame orientation in which vertices are expressed;
  *  assume that first 2 axis of inertia frame are parallel to the facet, and that 3rd axis is orthogonal to the facet
  *  The inertia of the triangle is same as if 1/3 of area was concentrated on each side mid point.
  *  \see Remarques sur la géométrie du triangle, E Cesaro, Nouvelles annales de mathématiques 3e série, tome 6 (1887), p. 215-242
  *  <http://www.numdam.org/item?id=NAM_1887_3_6__215_1>
  */
Eigen::Matrix3d inertia_of_triangle(
		const EPoint vertex1,  //!< first vertex of triangle expressed in inertia frame R1
		const EPoint vertex2,  //!< second vertex of triangle
		const EPoint vertex3   //!< third vertex of triangle
		);

/**  \details Compute the inertia matrix for a planar facet versus an inertia frame, divided by total area
  *  assume that first 2 axis of inertia frame are parallel to the facet, and that 3rd axis is orthogonal to the facet
  */
Eigen::Matrix3d inertia_of_polygon(
		const Matrix3x& verticesInR1  //!< polygon with vertices expressed in inertia frame R1
		);

/**  \brief Computes the volume inside a closed mesh.
  *  \returns Volume of the STL file (in m^3 if the unit in the STL data is m)
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest volume_example
  *  \see admesh-0.95
  *  \see Efficient feature extraction for 2D/3D objects in mesh representation, Cha Zhang and Tsuhan Chen, Dept. of Electrical and Computer Engineering, Carnegie Mellon University
  */
double volume(const VectorOfVectorOfPoints& mesh //!< Mesh we want to calculate the volume of
              );

/**  \brief Computes the volume inside a closed mesh defined by a list of facets
  *  \returns Volume of the STL file (in m^3 if the unit in the STL data is m)
  *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest volume_example
  *  \see admesh-0.95
  *  \see Efficient feature extraction for 2D/3D objects in mesh representation, Cha Zhang and Tsuhan Chen, Dept. of Electrical and Computer Engineering, Carnegie Mellon University
  */
double volume(const FacetIterator& begin, //!< Iterator to the beginning of the list of the facets defining the mesh
              const FacetIterator& end //!< Iterator to one-past the last facet in the list of the facets defining the mesh
              );

#endif /* MESH_MANIPULATIONS_HPP_ */
