#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
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
    Edge(size_t v1,size_t v2);

    /* \brief answer the (index of) first vertex encountered when running the edge in a given direction (0 or 1)
     */
    size_t first_vertex(int direction) const;

    /* \brief answer the (index of) second vertex encountered when running the edge in a given direction (0 or 1)
     */
    size_t second_vertex(int direction) const;

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
struct Mesh
{
    Mesh();
    Mesh(const Matrix3x& nodes_,
            const std::vector<Edge>& edges_,
            const std::vector<Facet>& facets_,
            const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
            const std::vector<std::vector<size_t> >& edgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
            const std::vector<std::vector<bool> >& edgesRunningDirection_, //!< for each Facet (index), the running direction of each edge
            const bool clockwise);

    /* brief Reset the dynamic data related to the mesh intersection with free surface */
    void reset_dynamic_data();

    Matrix3x nodes;            //!< Coordinates of static vertices in mesh
    std::vector<Edge> edges;   //!< All edges in mesh
    std::vector<Facet> facets; //!< For each facet, the indexes of its nodes, unit normal, barycenter & area
    std::vector<std::vector<size_t> > facetsPerEdge; //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
    std::vector<std::vector<size_t> > edgesPerFacet; //!< for each Facet (index), the list of Edges (indices) composing the facet
    std::vector<std::vector<bool> > edgesRunningDirection; //!< for each Facet (index), the running direction of each edge
    size_t static_nodes;       //!< Number of static nodes
    size_t static_edges;       //!< Number of static edges
    size_t static_facets;      //!< Number of static facets
    Matrix3x all_nodes;        //!< Coordinates of all vertices in mesh, including dynamic ones added for free surface intersection
    size_t node_count;         //!< Total number of nodes used, including dynamic ones
    double orientation_factor; //!< -1 if the facet is orientation clockwise, +1 otherwise
};

typedef TR1(shared_ptr)<Mesh> MeshPtr;

#endif //MESH_HPP
