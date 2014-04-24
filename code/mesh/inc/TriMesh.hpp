#ifndef TRIMESH_HPP
#define TRIMESH_HPP

#include <vector>
#include "GeometricTypes3d.hpp"

/**
 * \author gj
 * \brief Contains a triangular mesh
 * \details
 * \ingroup mesh
 * \section ex1 Example
 * \snippet mesh/unit_tests/src/TriMeshTest.cpp TriMeshTest example
 * \section ex2 Expected output
 * \snippet mesh/unit_tests/src/TriMeshTest.cpp TriMeshTest expected output
 */
struct TriMesh
{
    TriMesh();
    TriMesh(const Eigen::Matrix<double,3,Eigen::Dynamic>& nodes, const std::vector<Facet>& facets);

    Eigen::Matrix<double,3,Eigen::Dynamic> nodes;
    std::vector<Facet> facets;
};

#endif //TRIMESH_HPP
