#ifndef TRIMESH_HPP_
#define TRIMESH_HPP_

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
    Point3dTriplet points;
    Xyz unit_normal;
};

typedef std::vector<Facet> VectorOfFacet;

#endif
