#ifndef GEOMETRICTYPES3D_HPP_
#define GEOMETRICTYPES3D_HPP_

#include <stddef.h>
#include <vector>
#include <Eigen/Dense>

typedef std::vector<Eigen::Vector3d> VectorOfVector3d;

struct Point3dTriplet
{
    Eigen::Vector3d p1;
    Eigen::Vector3d p2;
    Eigen::Vector3d p3;
};

typedef std::vector<Point3dTriplet> VectorOfPoint3dTriplet;

struct Facet
{
    size_t index[3];
    Eigen::Vector3d unit_normal;
    Eigen::Vector3d barycenter;
    double area;
};

typedef std::vector<Facet> VectorOfFacet;

#endif
