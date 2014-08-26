#include "MeshBuilder.hpp"
#include "Mesh.hpp"

Mesh::Mesh():nodes(Eigen::Matrix<double,3,Eigen::Dynamic>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Eigen::Matrix<double,3,Eigen::Dynamic>& nodes_, const std::vector<Facet>& facets_, const bool clockwise):nodes(nodes_),facets(facets_),
        orientation_factor(clockwise ? -1 : 1)
{
}
