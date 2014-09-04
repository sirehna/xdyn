#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <set>
#include <map>
#include "GeometricTypes3d.hpp"

#include "tr1_macros.hpp"
#include TR1INC(memory)

/**
 * \brief Contains an edge of a mesh
 * \details an Edge is holding the indices of the vertices in the mesh rather than duplicating the coordinates.
 * \ingroup mesh
 */
struct Edge
{
    Edge(size_t v1,size_t v2)
    {
        vertex_index[0]=v1;
        vertex_index[1]=v2;
    }

    size_t vertex_index[2];  //!< The index of the two vertices in the mesh
};

/**
 * \brief Contains a facet of a mesh
 * \details a Facet is holding the indices of the vertices in the mesh rather than duplicating the coordinates
 * \ingroup mesh
 */
struct Facet
{
    Facet():vertex_index(std::vector<size_t>()),unit_normal(Eigen::MatrixXd::Zero(3,1)),barycenter(Eigen::MatrixXd::Zero(3,1)),area(0) {}
    std::vector<size_t> vertex_index;
    Eigen::Vector3d unit_normal;
    Eigen::Vector3d barycenter;
    double area;
};

typedef std::vector<Facet> VectorOfFacet;


/**
 * \author gj
 * \brief Contains a triangular mesh
 * \ingroup mesh
 */
class Mesh
{
    Mesh();
public:
    Mesh(const Matrix3x& nodes_,
            const std::vector<Edge>& edges_,
            const std::vector<Facet>& facets_,
            const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
            const std::vector<std::vector<size_t> >& orientedEdgesPerFacet_,  //!< for each Facet (index), the list of Edges composing the facet and their running direction of each edge
            const bool clockwise);


    /* \brief Reset the dynamic data related to the mesh intersection with free surface */
    void reset_dynamic_data();

    /* \brief add an edge
     * \return the edge index */
    size_t add_edge(const size_t first_vertex_index,const size_t last_vertex_index);

    /* \brief add an vertex
     * \return the vertex index */
    size_t add_vertex(const EPoint &vertex_coords);

    /* \brief create a new facet dynamically
     * \return the facet index in facets vector */
    size_t create_facet_from_edges(
            const std::vector<size_t>& oriented_edge_list, //!< The list of edges and running direction composing the facet
            const EPoint &unit_normal                      //!< The unit_normal is shared in case of facet split, let's not recompute it
            );

    /* an oriented edge is composed of an edge_index (left shifted) and a boolean indicating if the order of vertices must be reversed (coded on low bit) */
    static size_t make_oriented_edge(size_t edge_index_,bool reverse_direction_);

    /* \brief return the index of first vertex of an oriented edge
     */
    size_t first_vertex_of_oriented_edge(size_t oriented_edge) const;

    /* \brief return the index of second vertex of an oriented edge
     */
    size_t second_vertex_of_oriented_edge(size_t oriented_edge) const;

    Matrix3x nodes;            //!< Coordinates of static vertices in mesh
    std::vector<Edge> edges;   //!< All edges in mesh
    std::vector<Facet> facets; //!< For each facet, the indexes of its nodes, unit normal, barycenter & area
    std::vector<std::vector<size_t> > facetsPerEdge; //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
    std::vector<std::vector<size_t> > orientedEdgesPerFacet; //!< for each Facet (index), the list of Edges composing the facet and running direction of each edge

    size_t static_nodes;       //!< Number of static nodes
    size_t static_edges;       //!< Number of static edges
    size_t static_facets;      //!< Number of static facets
    Matrix3x all_nodes;        //!< Coordinates of all vertices in mesh, including dynamic ones added for free surface intersection
    size_t node_count;         //!< Total number of nodes used, including dynamic ones
    double orientation_factor; //!< -1 if the facet is orientation clockwise, +1 otherwise
};

typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<const Mesh> const_MeshPtr;

#endif //MESH_HPP
