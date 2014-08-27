/*
 * mesh_manipulations.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

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
    const int n = (int)points.cols();
    double a = 0;
    for (int i = 2 ; i < n ; ++i)
    {
        a += area(points, 0, i-1, i);
    }
    return a;
}

Eigen::Vector3d barycenter(const Matrix3x& p)
{
    return p.rowwise().sum().array()/double(p.cols());
}

Eigen::Vector3d barycenter(const VectorOfVectorOfPoints& points //!< List of points
                          )
{
    double x = 0;
    double y = 0;
    double z = 0;
    size_t N = 0;
    const size_t n = points.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        const size_t p = points[i].size();
        for (size_t j = 0 ; j < p ; ++j)
        {
            x += points[i][j](0);
            y += points[i][j](1);
            z += points[i][j](2);
            ++N;
        }
    }
    return Eigen::Vector3d(x/double(N),y/double(N),z/double(N));
}

Eigen::Vector3d unit_normal(const Matrix3x& points)
{
    //const double sign = oriented_clockwise ? -1 : 1;
    if (points.cols() < 3)
    {
        std::stringstream ss;
        ss << "Need at least three points to define a surface: cannot compute normal vector. Input has "
           << points.cols() << " point";
        if (points.cols()>1) ss << "s";
        ss << ".";
       THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    const double x1 = points(0,1)-points(0,0);
    const double x2 = points(1,1)-points(1,0);
    const double x3 = points(2,1)-points(2,0);
    const double y1 = points(0,2)-points(0,0);
    const double y2 = points(1,2)-points(1,0);
    const double y3 = points(2,2)-points(2,0);
    const double A = x2*y3-x3*y2;
    const double B = x3*y1-x1*y3;
    const double C = x1*y2-x2*y1;

    const double norm = sqrt(A*A+B*B+C*C);
    if (norm<1000*std::numeric_limits<double>::epsilon()) return Eigen::Vector3d(0,0,0);
    return Eigen::Vector3d(A/norm,B/norm,C/norm);
}

Eigen::Vector3d centre_of_gravity(const Matrix3x& polygon //!< Polygon we wish to compute the centre of gravity of
                                 )
{
    const int n = (int)polygon.cols();
    Eigen::Vector3d areas_times_points(0,0,0);
    double areas = 0;
    for (int i = 2 ; i < n ; ++i)
    {
        const double S = area(polygon, 0, i-1, i);
        areas += S;
        areas_times_points += S*(polygon.col(0)+polygon.col(i-1)+polygon.col(i))/3.;
    }
    return areas_times_points/areas;
}

Matrix3x convert(const VectorOfPoints& v)
{
    Matrix3x ret(3,v.size());
    for (size_t j = 0 ; j < v.size() ; ++j)
    {
        ret.col((int)j) = v[j];
    }
    return ret;
}
#include "test_macros.hpp"
bool oriented_clockwise(const VectorOfVectorOfPoints& v, const EPoint& O)
{
    if (v.size() < 2) return true;

    size_t nb_of_clockwise = 0;
    size_t nb_of_anticlockwise = 0;
    bool first_facet_is_oriented_clockwise = oriented_clockwise(v.front(),O);
    if (first_facet_is_oriented_clockwise) nb_of_clockwise++;
    else nb_of_anticlockwise++;
    for (size_t i = 1 ; i < v.size() ; ++i)
    {
        const bool facet_i_is_oriented_clockwise = oriented_clockwise(v[i],O);
        if (facet_i_is_oriented_clockwise) nb_of_clockwise++;
        else nb_of_anticlockwise++;
    }
    if (nb_of_clockwise > 10*nb_of_anticlockwise) return true;
    if (nb_of_anticlockwise > 10*nb_of_clockwise) return false;
    std::stringstream ss;
    ss << "Not all facets have the same orientation: " << nb_of_clockwise << " facets seem to be oriented clockwise, but "
       << nb_of_anticlockwise << " facets seem to be oriented anticlockwise.";
    THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    return first_facet_is_oriented_clockwise;
}

bool oriented_clockwise(const VectorOfPoints& facet, //!< Points to convert
                        const EPoint& G //!< Point inside the volume (eg. its centre of gravity)
        )
{
    if (facet.size() < 3) return true;
    const Matrix3x M = convert(facet);
    const Eigen::Vector3d C = barycenter(M);
    const Eigen::Vector3d n = unit_normal(M);
    return n.dot(C-G)<=0;
}

void write_binary_stl(const VectorOfVectorOfPoints& stl, std::ostream& os)
{
    //const std::string header(80, ' ');
    //os.write(header.c_str(), 80);
    for (size_t i = 0 ; i < 80 ; ++i) os.put(1);
    const unsigned int nFaces = (unsigned int)stl.size();
    os.write(reinterpret_cast<const char*>(&nFaces), sizeof(nFaces));

    typedef unsigned short uint16;
    const uint16 spacer = 0;
    // Every Face is 50 Bytes: Normal(3*float), Vertices(9*float), 2 Bytes Spacer
    for (size_t i=0; i<nFaces; ++i)
    {
        Eigen::Matrix3d M;
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            for (size_t k = 0 ; k < 3 ; ++k) M(k,j) = stl[i][j](k);
        }
        const Eigen::Vector3d normal = unit_normal(M);
        float x = (float)normal(0);
        float y = (float)normal(1);
        float z = (float)normal(2);
        os.write(reinterpret_cast<const char*>(&x), 4);
        os.write(reinterpret_cast<const char*>(&y), sizeof(float));
        os.write(reinterpret_cast<const char*>(&z), sizeof(float));
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            float x = (float)M(0,j);
            float y = (float)M(1,j);
            float z = (float)M(2,j);
            os.write(reinterpret_cast<const char*>(&x), 4);
            os.write(reinterpret_cast<const char*>(&y), 4);
            os.write(reinterpret_cast<const char*>(&z), 4);
        }
        os.write(reinterpret_cast<const char*>(&spacer), 2);
    }
}
