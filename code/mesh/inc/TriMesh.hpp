#ifndef TRIMESH_HPP
#define MESH_HPP

#include "GeometricTypes3d.hpp"

class TriMeshBuilder;
class TriMesh
{
	public:
		TriMesh();
		TriMesh(const std::vector<Xyz>& nodes_, const std::vector<Facet>& facets_);
		TriMesh(const TriMeshBuilder& builder);
		TriMesh(const VectorOfPoint3dTriplet& v);
		std::vector<Xyz> nodes;
		std::vector<Facet> facets;
};

#endif
