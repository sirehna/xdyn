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

void write_binary_stl(const VectorOfVectorOfPoints& stl, std::ostream& os)
{
    //const std::string header(80, ' ');
    //os.write(header.c_str(), 80);
    for (size_t i = 0 ; i < 80 ; ++i) os.put(1);
    const unsigned int nFaces = stl.size();
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

Matrix3x convert(const VectorOfPoints& v)
{
    Matrix3x ret(3,v.size());
    for (size_t j = 0 ; j < v.size() ; ++j)
    {
        ret.col((int)j) = v[j];
    }
    return ret;
}
