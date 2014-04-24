#include "TriMeshBuilder.hpp"
#include "TriMesh.hpp"

TriMesh::TriMesh():nodes(Eigen::Matrix<double,3,Eigen::Dynamic>()),facets(std::vector<Facet>())
{
}

TriMesh::TriMesh(const Eigen::Matrix<double,3,Eigen::Dynamic>& nodes_, const std::vector<Facet>& facets_):nodes(nodes_),facets(facets_)
{
}
