#ifndef GEOMETRICTYPES3D_HPP_
#define GEOMETRICTYPES3D_HPP_

#include <stddef.h>
#include <vector>
#include <Eigen/Dense>

typedef std::vector<Eigen::Vector3d> VectorOfVector3d;
typedef Eigen::Matrix<double,3,Eigen::Dynamic> Matrix3Xd;

struct Point3dTriplet
{
    Point3dTriplet():p1(Eigen::Vector3d()),p2(Eigen::Vector3d()),p3(Eigen::Vector3d()){}
    Eigen::Vector3d p1;
    Eigen::Vector3d p2;
    Eigen::Vector3d p3;
};

typedef std::vector<Point3dTriplet> VectorOfPoint3dTriplet;

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
