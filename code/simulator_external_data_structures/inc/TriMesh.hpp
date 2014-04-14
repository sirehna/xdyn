/*
 * TriMesh.hpp
 *
 *  Created on: 14 avr. 2014
 *      Author: jacquenot
 */

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

#endif /* TRIMESH_HPP_ */
