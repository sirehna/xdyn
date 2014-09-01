#include "MeshBuilder.hpp"
#include "Mesh.hpp"

Edge::Edge(size_t v1,size_t v2)
{
    vertex_index[0]=v1;
    vertex_index[1]=v2;
}

size_t Edge::first_edge(int direction) const
{
    return vertex_index[direction];
}

size_t Edge::last_edge(int direction) const
{
    return vertex_index[1-direction];
}


Mesh::Mesh():nodes(Matrix3x()),edges(std::vector<Edge>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Matrix3x& nodes_, const std::vector<Edge>& edges_, const std::vector<Facet>& facets_, const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,orientation_factor(clockwise ? -1 : 1)
{
}
