/*
 * mesh_manipulations.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#include "mesh_manipulations.hpp"
#include "MeshException.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>

using namespace boost::geometry;

BOOST_GEOMETRY_REGISTER_POINT_3D(EPoint, double, cs::cartesian, x, y, z)

double area(const VectorOfPoints& polygon)
{
    const Eigen::Vector3d n1(polygon[1]-polygon[0]);
    const Eigen::Vector3d n2(polygon[2]-polygon[0]);
    return 0.5*fabs((n1.cross(n2)).norm());
}

Eigen::Vector3d barycenter(const VectorOfPoints& polygon)
{
    Eigen::Vector3d xyz;
    xyz = (polygon[0]+polygon[1]+polygon[2])/3.0;
    return xyz;
}

Eigen::Vector3d unit_normal(const VectorOfPoints& polygon)
{
    const Eigen::Vector3d n = normal(polygon);
    const double norm = n.norm();
    if (norm<1000*std::numeric_limits<double>::epsilon())
    {
        std::stringstream ss;
        ss << "Input is degenerated: cannot compute unit normal vector. The polygon is:" << std::endl;
        for (size_t i = 0 ; i < polygon.size() ; ++i)
        {
          ss << "p[" << i << "] = " << polygon[i].transpose() << std::endl;
        }
        THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    return n/norm;
}

Eigen::Vector3d normal(const VectorOfPoints& polygon)
{
    if (polygon.size() < 3)
    {
        std::stringstream ss;
        ss << "Need at least three points to define a surface: cannot compute normal vector. Input has "
           << polygon.size() << " points.";
       THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    const Eigen::Vector3d n1(polygon[1]-polygon[0]);
    const Eigen::Vector3d n2(polygon[2]-polygon[0]);
    return n1.cross(n2);
}
