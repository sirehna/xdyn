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
    Edge(size_t v1,size_t v2);

    /* \brief answer the (index of) first vertex encountered when running the edge in a given direction (0 or 1)
     */
    size_t first_vertex(int direction) const;

    /* \brief answer the (index of) second vertex encountered when running the edge in a given direction (0 or 1)
     */
    size_t second_vertex(int direction) const;

    /* \brief answer whether this edge crosses the free surface
     */
    bool touches_free_surface() const;

    /* \brief answer whether this edge is totally emerged
     */
    bool is_emerged() const;

    /* \brief answer whether this edge is totally immersed
     */
    bool is_immersed() const;

    /* \brief reset the status of the edge w.r.t. free surface
     * \return true if this edge crosses the free surface
     */
    void update_intersection_with_free_surface(
             const std::vector<double>& relative_immersions //!< the relative immersion of each static vertex of the mesh
             );

    size_t vertex_index[2];  //!< The index of the two vertices in the mesh
    unsigned char status;    //!< 2 bits of immersion status: 00=totally emerged, 01=second emerged,first immersed, 10=first emerged,second immersed, 11=totally immersed
                             // + third bit = 1 for potentially crossing (if any of the vertices is exactly on free surface)
};


/**
 * \brief Reference to an edge, with a running direction.
 * \details Usefull to describe the edges of a facet.
 * \ingroup mesh
 */
struct OrientedEdge
{
    OrientedEdge(size_t edge_index_,bool dir):edge_index(edge_index_),direction(dir?1:0) {}
    size_t edge_index;
    int    direction;
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
            const std::vector<std::vector<OrientedEdge> >& edgesPerFacet_,  //!< for each Facet (index), the list of Edges composing the facet and their running direction of each edge
            const bool clockwise);

    /* \brief Reset the dynamic data related to the mesh intersection with free surface */
    void reset_dynamic_data();

    /* \brief Update the intersection of the mesh with free surface
     * \details the intersection requires new Vertices/Edges/Facets stored as dynamic data in the end of container members */
    void update_intersection_with_free_surface(
            const std::vector<double>& relative_immersions //!< the relative immersion of each static vertex of the mesh
            );

    /* \brief Extract the coordinates of a specific facet */
    Matrix3x coordinates_of_facet(size_t facet_index) const;

    /* \brief Extract the relative immersions of a specific facet */
    std::vector<double> immersions_of_facet(size_t facet_index) const;

    /* \brief Compute the point of intersection with free surface between two vertices
     * \details One of the vertices must be emerged and the other immersed */
    EPoint edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB) const;

    /* \brief split an edge into an emerged and an immersed part */
    size_t split_partially_immersed_edge(const Edge &edge);

    /* \brief split a Facet into an emerged and an immersed part */
    void split_partially_immersed_facet(
            size_t facet_index,
            const std::map<size_t,size_t >& added_edges);

    /* \brief create a new facet dynamically
     * \return the facet index in facets vector */
    size_t create_facet_from_edges(const std::vector<OrientedEdge>& edge_list);

    Matrix3x nodes;            //!< Coordinates of static vertices in mesh
    std::vector<Edge> edges;   //!< All edges in mesh
    std::vector<Facet> facets; //!< For each facet, the indexes of its nodes, unit normal, barycenter & area
    std::vector<std::vector<size_t> > facetsPerEdge; //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
    std::vector<std::vector<OrientedEdge> > edgesPerFacet; //!< for each Facet (index), the list of Edges composing the facet and running direction of each edge

    size_t static_nodes;       //!< Number of static nodes
    size_t static_edges;       //!< Number of static edges
    size_t static_facets;      //!< Number of static facets
    Matrix3x all_nodes;        //!< Coordinates of all vertices in mesh, including dynamic ones added for free surface intersection
    size_t node_count;         //!< Total number of nodes used, including dynamic ones
    double orientation_factor; //!< -1 if the facet is orientation clockwise, +1 otherwise

    std::vector<double> all_immersions; //<! the immersions of all nodes (including the dynamically added ones)
    std::set<size_t> set_of_facets_crossing_free_surface; //!< list of facets to be split into an emerged and immersed parts
    std::vector<size_t> list_of_facets_emerged;  //!< list of all emerged facets (included the dynamically ones created by split)
    std::vector<size_t> list_of_facets_immersed; //!< list of all immersed facets (included the dynamically ones created by split)
};

typedef TR1(shared_ptr)<Mesh> MeshPtr;
typedef TR1(shared_ptr)<const Mesh> const_MeshPtr;

#endif //MESH_HPP
