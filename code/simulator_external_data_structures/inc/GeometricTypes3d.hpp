#ifndef GEOMETRICTYPES3D_HPP_
#define GEOMETRICTYPES3D_HPP_

#include <stddef.h>
#include <vector>

struct Xyz
{
    double x;
    double y;
    double z;
};

struct Point3dTriplet
{
    Xyz p1;
    Xyz p2;
    Xyz p3;
};

typedef std::vector<Point3dTriplet> VectorOfPoint3dTriplet;

struct Facet
{
    size_t index[3];
    Xyz unit_normal;
    Xyz barycenter;
    double area;
};

typedef std::vector<Facet> VectorOfFacet;

#endif
