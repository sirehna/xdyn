
#include "Mesh.hpp"
#include "mesh_manipulations.hpp"

Mesh::Mesh():nodes(Matrix3x()),edges(std::vector<Edge>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Matrix3x& nodes_,
        const std::vector<Edge>& edges_,
        const std::vector<Facet>& facets_,
        const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
        const std::vector<std::vector<size_t> >& orientedEdgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
        const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,facetsPerEdge(facetsPerEdge_)
,orientedEdgesPerFacet(orientedEdgesPerFacet_)
,static_nodes(nodes_.cols())
,static_edges(edges_.size())
,static_facets(facets_.size())
,all_nodes(3,static_nodes+static_edges)
,node_count(static_nodes)
,orientation_factor(clockwise ? -1 : 1)
{
    Matrix3x room_for_dynamic_vertices(3,all_nodes.cols()-nodes.cols());
    room_for_dynamic_vertices.fill(0);
    all_nodes << nodes , room_for_dynamic_vertices;
}

void Mesh::reset_dynamic_data()
{
    node_count = static_nodes;
    edges.erase( edges.begin() + static_edges , edges.end());
    facets.erase( facets.begin() + static_facets , facets.end());
}

size_t Mesh::create_facet_from_edges(const std::vector<size_t>& oriented_edge_list,const EPoint &unit_normal)
{
    std::vector<size_t> vertex_list(oriented_edge_list.size(),0);
    Matrix3x coords(3,oriented_edge_list.size());
    for( size_t ei=0;ei<oriented_edge_list.size();ei++) {
        size_t vertex_index = second_vertex_of_oriented_edge(oriented_edge_list[ei]); // Note: use second vertex rather than first for compatibility with existing tests
        vertex_list[ei]=vertex_index;
        coords.col(ei)=all_nodes.col(vertex_index);
    }
    size_t facet_index = facets.size();
    facets.push_back(Facet(vertex_list,unit_normal,::barycenter(coords),::area(coords)));
    return facet_index;
}

size_t Mesh::add_edge(const size_t first_vertex_index,const size_t last_vertex_index)
{
    edges.push_back(Edge(first_vertex_index,last_vertex_index));
    return edges.size()-1; // that is the index of first_sub_edge added
}

size_t Mesh::add_vertex(const EPoint &vertex_coords)
{
    size_t node_index = node_count;
    all_nodes.col(node_index) = vertex_coords;
    ++node_count;
    return node_index;
}

size_t Mesh::make_oriented_edge(size_t edge_index,bool reverse_direction)
{
    return (edge_index<<1) | (reverse_direction?1:0);
}

size_t Mesh::get_oriented_edge_index(size_t oriented_edge)
{
    return oriented_edge >> 1;
}

bool Mesh::get_oriented_edge_direction(size_t oriented_edge)
{
    return (oriented_edge & 1) != 0;
}

/* \brief return the first vertex of an oriented edge
 */
size_t Mesh::first_vertex_of_oriented_edge(size_t oriented_edge) const
{
    size_t edge_index = oriented_edge >> 1;
    size_t reverse_direction = oriented_edge & 1;
    return edges[edge_index].vertex_index[reverse_direction];
}

/* \brief return the second vertex of an oriented edge
 */
size_t Mesh::second_vertex_of_oriented_edge(size_t oriented_edge) const
{
    size_t edge_index = oriented_edge >> 1;
    size_t reverse_direction = oriented_edge & 1;
    return edges[edge_index].vertex_index[1-reverse_direction];
}

