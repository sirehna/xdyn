/*
 * mesh_manipulations.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#include "mesh_manipulations.hpp"
#include "MeshException.hpp"
#include "kahan_sum.hpp"
#include "pairwise_sum.hpp"

double triangle_area(const EPoint& A, const EPoint& B, const EPoint& C)
{
    return 0.5*fabs(((B-A).cross(C-A)).norm());
}

double area(const Matrix3x& points)
{
    const int n = points.cols();
    double a = 0;
    for (int i = 2 ; i < n ; ++i)
    {
        a += triangle_area(points.col(0), points.col(i-1), points.col(i));
    }
    return a;
}

Eigen::Vector3d barycenter(const Matrix3x& p)
{
    std::vector<Eigen::Vector3d> points;
    for (int j = 0 ; j < p.cols() ; ++j)
    {
        points.push_back(p.col(j));
    }
    return sum::pairwise(points)/double(points.size());
}

Eigen::Vector3d unit_normal(const Matrix3x& points)
{
    if (points.cols() < 3)
    {
        std::stringstream ss;
        ss << "Need at least three points to define a surface: cannot compute normal vector. Input has "
           << points.cols() << " point";
        if (points.cols()>1) ss << "s";
        ss << ".";
       THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    const Eigen::Vector3d n1(points.col(1)-points.col(0));
    const Eigen::Vector3d n2(points.col(2)-points.col(0));
    const Eigen::Vector3d n = n1.cross(n2);
    const double norm = n.norm();
    if (norm<1000*std::numeric_limits<double>::epsilon())
    {
        std::stringstream ss;
        ss << "Input is degenerated: cannot compute unit normal vector. The polygon is:" << std::endl;
        for (int j = 0 ; j < points.cols() ; ++j)
        {
          ss << "p[" << j << "] = " << points.col(j).transpose() << std::endl;
        }
        THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    return n/norm;
}

Eigen::Vector3d centre_of_gravity(const Matrix3x& polygon //!< Polygon we wish to compute the centre of gravity of
                                 )
{
    const int n = polygon.cols();
    std::vector<Eigen::Vector3d> areas_times_points;
    std::vector<double> areas;
    for (int i = 2 ; i < n ; ++i)
    {
        areas.push_back(triangle_area(polygon.col(0), polygon.col(i-1), polygon.col(i)));
        areas_times_points.push_back(areas.back()*(polygon.col(0)+polygon.col(i-1)+polygon.col(i))/3.);
    }
    return sum::pairwise(areas_times_points)/sum::pairwise(areas);
}
