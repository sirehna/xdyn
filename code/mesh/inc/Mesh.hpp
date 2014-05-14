#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "GeometricTypes3d.hpp"

/**
 * \author gj
 * \brief Contains a triangular mesh
 * \ingroup mesh
 */
struct Mesh
{
    Mesh();
    Mesh(const Eigen::Matrix<double,3,Eigen::Dynamic>& nodes, const std::vector<Facet>& facets);

    Eigen::Matrix<double,3,Eigen::Dynamic> nodes;
    std::vector<Facet> facets;
};

#endif //MESH_HPP
