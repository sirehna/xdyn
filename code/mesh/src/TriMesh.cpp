#include "TriMeshBuilder.hpp"
#include "TriMesh.hpp"

TriMesh::TriMesh():nodes(std::vector<Eigen::Vector3d>()),facets(std::vector<Facet>())
{
}

TriMesh::TriMesh(const std::vector<Eigen::Vector3d>& nodes_, const std::vector<Facet>& facets_):nodes(nodes_),facets(facets_)
{
}

TriMesh::TriMesh(const VectorOfPoint3dTriplet& v):nodes(std::vector<Eigen::Vector3d>()),facets(std::vector<Facet>())
{
	TriMeshBuilder builder(v);
	builder.build();
	nodes = builder.get_nodes();
	facets = builder.get_facets();
}
