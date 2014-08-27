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
    Mesh(const Matrix3x& nodes, const std::vector<Facet>& facets, const bool clockwise);

    Matrix3x nodes;            //!< All points in mesh
    std::vector<Facet> facets; //!< For each facet, the indexes of its nodes, unit normal, barycenter & area
    double orientation_factor; // -1 if the facet is orientation clockwise, +1 otherwise
};

#endif //MESH_HPP
