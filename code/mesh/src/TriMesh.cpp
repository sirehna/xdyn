#include "TriMeshBuilder.hpp"
#include "TriMesh.hpp"

TriMesh::TriMesh():nodes(std::vector<Xyz>()),facets(std::vector<Facet>())
{
}

TriMesh::TriMesh(const std::vector<Xyz>& nodes_, const std::vector<Facet>& facets_):nodes(nodes_),facets(facets_)
{
}

TriMesh::TriMesh(const TriMeshBuilder& builder):nodes(builder.get_nodes()),facets(builder.get_facets())
{
}

TriMesh::TriMesh(const VectorOfPoint3dTriplet& v):nodes(std::vector<Xyz>()),facets(std::vector<Facet>())
{
	TriMeshBuilder builder(v);
	builder.build();
	nodes = builder.get_nodes();
	facets = builder.get_facets();
}

/*
 *         double calcArea() const {
            rw::math::Vector3D<T> ab = getVertex(1)-getVertex(0);
            rw::math::Vector3D<T> ac = getVertex(2)-getVertex(0);
            return rw::math::MetricUtil::norm2( cross(ab,ac) )/2;
        }
 */
