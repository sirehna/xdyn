#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "GeometricTypes3d.hpp"

#include "tr1_macros.hpp"
#include TR1INC(memory)

/**
 * \brief Contains an edge of a mesh
 * \ingroup mesh
 */
struct Edge
{
public:
    Edge(size_t v1,size_t v2);
    size_t first_edge(int direction) const;
    size_t last_edge(int direction) const;
    size_t vertex_index[2];  //!< The index of the two vertices in the mesh
};


/**
 * \brief Contains a facet of a mesh
 * \ingroup mesh
 */
struct Facet
{
    Facet():index(std::vector<size_t>()),unit_normal(Eigen::MatrixXd::Zero(3,1)),barycenter(Eigen::MatrixXd::Zero(3,1)),area(0) {}
    std::vector<size_t> index;
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
    Mesh(const Matrix3x& nodes_, const std::vector<Edge>& edges_, const std::vector<Facet>& facets_, const bool clockwise);

    Matrix3x nodes;            //!< All points in mesh
    std::vector<Edge> edges;   //!< All edges in mesh
    std::vector<Facet> facets; //!< For each facet, the indexes of its nodes, unit normal, barycenter & area
    double orientation_factor; // -1 if the facet is orientation clockwise, +1 otherwise
};

typedef TR1(shared_ptr)<Mesh> MeshPtr;

#endif //MESH_HPP
