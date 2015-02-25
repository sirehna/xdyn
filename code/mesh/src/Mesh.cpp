#include <map>

#include "Mesh.hpp"
#include "mesh_manipulations.hpp"

Mesh::Mesh():
    nodes(),
    edges(),
    facets(),
    facets_per_edge(),
    oriented_edges_per_facet(),
    nb_of_static_nodes(),
    nb_of_static_edges(),
    nb_of_static_facets(),
    all_nodes(),
    total_number_of_nodes(),
    orientation_factor(1)
{
}

Mesh::Mesh(
        const Matrix3x& nodes_,
        const ArrayOfEdges& edges_,
        const std::vector<Facet>& facets_,
        const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
        const std::vector<std::vector<size_t> >& orientedEdgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
        const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,facets_per_edge(facetsPerEdge_)
,oriented_edges_per_facet(orientedEdgesPerFacet_)
,nb_of_static_nodes((size_t)nodes_.cols())
,nb_of_static_edges(edges_[0].size())
,nb_of_static_facets(facets_.size())
,all_nodes(3,nb_of_static_nodes+nb_of_static_edges)
,total_number_of_nodes(nb_of_static_nodes)
,orientation_factor(clockwise ? -1 : 1)
{
    Matrix3x room_for_dynamic_vertices(3,all_nodes.cols()-nodes.cols());
    room_for_dynamic_vertices.fill(0);
    all_nodes << nodes , room_for_dynamic_vertices;
}

void Mesh::reset_dynamic_data()
{
    total_number_of_nodes = nb_of_static_nodes;
    edges[0].erase( edges[0].begin() + (int)nb_of_static_edges , edges[0].end());
    edges[1].erase( edges[1].begin() + (int)nb_of_static_edges , edges[1].end());
    facets.erase( facets.begin() + (int)nb_of_static_facets , facets.end());
}

size_t Mesh::create_facet_from_edges(const std::vector<size_t>& oriented_edge_list,const EPoint &unit_normal)
{
    size_t n=oriented_edge_list.size();
    std::vector<size_t> vertex_list(n,0);
    std::map<size_t, bool> vertex_inserted;
    size_t nb_of_vertices = 0;
    for( size_t ei=0;ei<oriented_edge_list.size();ei++)
    {
        size_t vertex_index = second_vertex_of_oriented_edge(oriented_edge_list[ei]); // Note: use second vertex rather than first for compatibility with existing tests
        if (not(vertex_inserted[vertex_index]))
        {
            vertex_list[ei]=vertex_index;
            vertex_inserted[vertex_index] = true;
            nb_of_vertices++;
        }
    }
    vertex_list.resize(nb_of_vertices);
    size_t facet_index = facets.size();
    facets.push_back(Facet(vertex_list,unit_normal,::centre_of_gravity(all_nodes,vertex_list),::area(all_nodes,vertex_list)));
    return facet_index;
}

size_t Mesh::add_edge(const size_t first_vertex_index,const size_t last_vertex_index)
{
    edges[0].push_back(first_vertex_index);
    edges[1].push_back(last_vertex_index);
    return edges[0].size()-1; // that is the index of first_sub_edge added
}

size_t Mesh::add_vertex(const EPoint &vertex_coords)
{
    size_t node_index = total_number_of_nodes;
    all_nodes.col((int)node_index) = vertex_coords;
    ++total_number_of_nodes;
    return node_index;
}

size_t Mesh::make_oriented_edge(size_t edge_index,bool reverse_direction)
{
    return (edge_index<<1) | (reverse_direction?1:0);
}

size_t Mesh::get_oriented_edge_index(size_t node_index)
{
    // Divide by two to obtain index of corresponding edge
    // because there are exactly two nodes per edge
    return node_index >> 1;
}

bool Mesh::get_oriented_edge_direction(size_t oriented_edge)
{
    return (oriented_edge & 1) != 0;
}

/**
 * \brief return the first vertex of an oriented edge
 */
size_t Mesh::first_vertex_of_oriented_edge(size_t oriented_edge) const
{
    size_t edge_index = oriented_edge >> 1;
    size_t reverse_direction = oriented_edge & 1;
    return edges[reverse_direction][edge_index];
}

/**
 * \brief return the second vertex of an oriented edge
 */
size_t Mesh::second_vertex_of_oriented_edge(size_t oriented_edge) const
{
    size_t edge_index = oriented_edge >> 1;
    size_t reverse_direction = oriented_edge & 1;
    return edges[1-reverse_direction][edge_index];
}

