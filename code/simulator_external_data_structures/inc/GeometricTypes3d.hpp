#ifndef GEOMETRICTYPES3D_HPP_
#define GEOMETRICTYPES3D_HPP_

#include <stddef.h>
#include <vector>
#include <Eigen/Dense>


typedef std::vector<Eigen::Vector3d> VectorOfPoints;
typedef std::vector<VectorOfPoints> VectorOfVectorOfPoints;

struct Facet
{
    Facet():index(),unit_normal(Eigen::Vector3d()),barycenter(Eigen::Vector3d()),area(0.0){}
    size_t index[3];
    Eigen::Vector3d unit_normal;
    Eigen::Vector3d barycenter;
    double area;
};

typedef std::vector<Facet> VectorOfFacet;

#endif
