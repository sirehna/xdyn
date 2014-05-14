#include "MeshBuilder.hpp"
#include "Mesh.hpp"

Mesh::Mesh():nodes(Eigen::Matrix<double,3,Eigen::Dynamic>()),facets(std::vector<Facet>())
{
}

Mesh::Mesh(const Eigen::Matrix<double,3,Eigen::Dynamic>& nodes_, const std::vector<Facet>& facets_):nodes(nodes_),facets(facets_)
{
}
