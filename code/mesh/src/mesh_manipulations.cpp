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


double area(const Matrix3x& M, //!< Matrix containing (amongst others), the points of interest
            const int idxA,         //!< Index of the column containing the first point
            const int idxB,         //!< Index of the column containing the second point
            const int idxC          //!< Index of the column containing the third point
            )
{
    const double x1 = M(0,idxB)-M(0,idxA);
    const double x2 = M(1,idxB)-M(1,idxA);
    const double x3 = M(2,idxB)-M(2,idxA);
    const double y1 = M(0,idxC)-M(0,idxA);
    const double y2 = M(1,idxC)-M(1,idxA);
    const double y3 = M(2,idxC)-M(2,idxA);
    const double A = x2*y3-x3*y2;
    const double B = x3*y1-x1*y3;
    const double C = x1*y2-x2*y1;
    return 0.5*sqrt(A*A+B*B+C*C);
}

double area(const Matrix3x& points)
{
    const int n = points.cols();
    double a = 0;
    for (int i = 2 ; i < n ; ++i)
    {
        a += area(points, 0, i-1, i);
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
        areas.push_back(area(polygon, 0, i-1, i));
        areas_times_points.push_back(areas.back()*(polygon.col(0)+polygon.col(i-1)+polygon.col(i))/3.);
    }
    return sum::pairwise(areas_times_points)/sum::pairwise(areas);
}
