#ifndef GEOMETRICTYPES3D_HPP_
#define GEOMETRICTYPES3D_HPP_

#include <cstddef>
#include <vector>

#include <Eigen/Dense>

struct EPoint : public Eigen::Vector3d
{
    EPoint() : Eigen::Vector3d(), x((*this)[0]), y((*this)[1]), z((*this)[2])
    {
    }

    EPoint(const double x_, const double y_, const double z_) : Eigen::Vector3d(x_,y_,z_), x((*this)[0]), y((*this)[1]), z((*this)[2])
    {
    }

    EPoint(const EPoint& rhs) : Eigen::Vector3d(rhs), x((*this)[0]), y((*this)[1]), z((*this)[2])
    {
    }

    EPoint& operator=(const EPoint& rhs)
    {
        if (&rhs != this)
        {
            (*this)[0] = rhs[0];
            (*this)[1] = rhs[1];
            (*this)[2] = rhs[2];
            x = (*this)[0];
            y = (*this)[1];
            z = (*this)[2];
        }
        return *this;
    }

    double& x;
    double& y;
    double& z;
};

typedef std::vector<EPoint> VectorOfPoints;
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
