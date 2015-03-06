#ifndef MESH_HPP
#define MESH_HPP

#include <array>
#include <vector>
#include "GeometricTypes3d.hpp"

#include <ssc/macros.hpp>
#include TR1INC(memory)

typedef std::array<std::vector<size_t>,2> ArrayOfEdges;

/**
 * \details Holds the indices of the vertices in the mesh rather than duplicating the coordinates
 * \ingroup mesh
 */
struct Facet
{
    Facet():vertex_index(),unit_normal(Eigen::MatrixXd::Zero(3,1)),centre_of_gravity(Eigen::MatrixXd::Zero(3,1)),area(0) {}
    Facet(const std::vector<size_t> &vertex_index_,const Eigen::Vector3d &unit_normal_,const Eigen::Vector3d &barycenter_,double area_)
    :vertex_index(vertex_index_)
    ,unit_normal(unit_normal_)
    ,centre_of_gravity(barycenter_)
    ,area(area_) {}
    std::vector<size_t> vertex_index;
    Eigen::Vector3d unit_normal;
    Eigen::Vector3d centre_of_gravity;
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
         const ArrayOfEdges& edges_,
         const std::vector<Facet>& facets_,
         const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
         const std::vector<std::vector<size_t> >& orientedEdgesPerFacet_,  //!< for each Facet (index), the list of Edges composing the facet and their running direction of each edge
         const bool clockwise);


    /** \brief Reset the dynamic data related to the mesh intersection with free surface */
    void reset_dynamic_data();

    /** \brief add an edge
     * \return the edge index
     */
    size_t add_edge(const size_t first_vertex_index,const size_t last_vertex_index);

    /** \brief add an vertex
     * \return the vertex index
     */
    size_t add_vertex(const EPoint &vertex_coords);

    /**
     * \brief create a new facet dynamically
     * \return the facet index in facets vector
     */
    size_t create_facet_from_edges(
            const std::vector<size_t>& oriented_edge_list, //!< The list of edges and running direction composing the facet
            const EPoint &unit_normal                      //!< The unit_normal is shared in case of facet split, let's not recompute it
            );

    /**
     * \brief make an oriented edge
     * \details an oriented edge is composed of an edge_index and a boolean indicating if the order of vertices must be reversed
     * To avoid ctors/dtors, this is encoded in a single int, index being left shifted, and direction encoded on low bit
     */
    static size_t convert_index_to_oriented_edge_id(size_t edge_index_,bool reverse_direction_);
    static size_t convert_oriented_edge_id_to_edge_index(size_t node_index);
    static bool get_oriented_edge_direction(size_t oriented_edge);

    /**
     * \brief return the index of first vertex of an oriented edge
     */
    size_t first_vertex_of_oriented_edge(size_t oriented_edge) const;

    /**
     * \brief return the index of second vertex of an oriented edge
     */
    size_t second_vertex_of_oriented_edge(size_t oriented_edge) const;

    Matrix3x nodes;                                             //!< Coordinates of static vertices in mesh
    ArrayOfEdges edges;                                         //!< All edges in mesh
    std::vector<Facet> facets;                                  //!< For each facet, the indexes of its nodes, unit normal, barycenter & area
    std::vector<std::vector<size_t> > facets_per_edge;          //!< For each Edge (index), the list of Facet (indices) to which the edge belongs
    std::vector<std::vector<size_t> > oriented_edges_per_facet; //!< For each Facet (index), the list of Edges composing the facet and running direction of each edge
    size_t nb_of_static_nodes;                                  //!< Number of static nodes (ie. read from an STL file & not generated dynamically)
    size_t nb_of_static_edges;                                  //!< Number of static edges (ie. read from an STL file & not generated dynamically)
    size_t nb_of_static_facets;                                 //!< Number of static facets (ie. read from an STL file & not generated dynamically)
    Matrix3x all_nodes;                                         //!< Coordinates of all vertices in mesh, including dynamic ones added for free surface intersection
    size_t total_number_of_nodes;                               //!< Total number of nodes used, including dynamic ones
    double orientation_factor;                                  //!< -1 if the facet is orientation clockwise, +1 otherwise
};

typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<const Mesh> const_MeshPtr;

#endif //MESH_HPP
