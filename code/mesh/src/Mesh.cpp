#include "MeshBuilder.hpp"
#include "Mesh.hpp"

Edge::Edge(size_t v1,size_t v2)
{
    vertex_index[0]=v1;
    vertex_index[1]=v2;
}

size_t Edge::first_vertex(int direction) const
{
    return vertex_index[direction];
}

size_t Edge::second_vertex(int direction) const
{
    return vertex_index[1-direction];
}


Mesh::Mesh():nodes(Matrix3x()),edges(std::vector<Edge>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Matrix3x& nodes_,
        const std::vector<Edge>& edges_,
        const std::vector<Facet>& facets_,
        const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
        const std::vector<std::vector<size_t> >& edgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
        const std::vector<std::vector<bool> >& edgesRunningDirection_, //!< for each Facet (index), the running direction of each edge
        const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,facetsPerEdge(facetsPerEdge_)
,edgesPerFacet(edgesPerFacet_)
,edgesRunningDirection(edgesRunningDirection_)
,static_nodes(nodes_.cols())
,static_edges(edges_.size())
,static_facets(facets_.size())
,all_nodes(3,static_nodes+static_edges)
,node_count(static_nodes)
,orientation_factor(clockwise ? -1 : 1)
{
    Matrix3x room_for_dynamic_vertices(3,static_edges);
    room_for_dynamic_vertices.fill(0);
    all_nodes << nodes , room_for_dynamic_vertices;
}

void Mesh::reset_dynamic_data()
{
    node_count = static_nodes;
    edges.erase( edges.begin() + static_edges , edges.end());
    facets.erase( facets.begin() + static_facets , facets.end());
}
