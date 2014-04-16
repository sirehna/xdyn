#ifndef TRIMESH_HPP
#define MESH_HPP

#include <vector>
#include "GeometricTypes3d.hpp"

class TriMeshBuilder;

/**
 * \author gj
 * \brief Contains a triangular mesh
 * \details
 * \ingroup mesh
 * \section ex1 Example
 * \snippet mesh/unit_tests/src/TriMeshTest.cpp TriMeshTest example
 * \section ex2 Expected output
 * \snippet mesh/unit_tests/src/TriMeshTest.cpp TriMeshTest expected output
 */
class TriMesh
{
	public:
		TriMesh();
		TriMesh(const std::vector<Eigen::Vector3d>& nodes_, const std::vector<Facet>& facets_);
		TriMesh(const TriMeshBuilder& builder);
		TriMesh(const Point3dTriplet& tri);
		TriMesh(const VectorOfPoint3dTriplet& v);
		std::vector<Eigen::Vector3d> nodes;
		std::vector<Facet> facets;
};

#endif
