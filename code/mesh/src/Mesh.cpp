
#include "Mesh.hpp"
#include "mesh_manipulations.hpp"

Edge::Edge(size_t v1,size_t v2)
{
    vertex_index[0]=v1;
    vertex_index[1]=v2;
}

size_t Edge::first_vertex(bool reverse_direction) const
{
    return vertex_index[reverse_direction];
}

size_t Edge::second_vertex(bool reverse_direction) const
{
    return vertex_index[1-reverse_direction];
}


Mesh::Mesh():nodes(Matrix3x()),edges(std::vector<Edge>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Matrix3x& nodes_,
        const std::vector<Edge>& edges_,
        const std::vector<Facet>& facets_,
        const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
        const std::vector<std::vector<OrientedEdge> >& edgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
        const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,facetsPerEdge(facetsPerEdge_)
,edgesPerFacet(edgesPerFacet_)
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

size_t Mesh::create_facet_from_edges(const std::vector<OrientedEdge>& edge_list,const EPoint &unit_normal)
{
    Facet added_facet;
    std::vector<size_t> vertex_list;
    Matrix3x coords(3,edge_list.size());
    for( size_t ei=0;ei<edge_list.size();ei++) {
        size_t vertex_index = edges[edge_list[ei].edge_index].second_vertex(edge_list[ei].reverse_direction); // Note: use second vertex rather than first for compatibility with existing tests
        vertex_list.push_back(vertex_index);
        coords.col(ei)=all_nodes.col(vertex_index);
    }
    added_facet.vertex_index = vertex_list;
    added_facet.area         = ::area(coords);
    added_facet.unit_normal  = unit_normal;
    added_facet.barycenter   = ::barycenter(coords);
    size_t facet_index = facets.size();
    facets.push_back(added_facet);
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

