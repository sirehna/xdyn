#include <sstream>

#include "stl_writer.hpp"
#include "MeshException.hpp"

::std::ostream& operator<<(::std::ostream& os, const EPoint& point);
::std::ostream& operator<<(::std::ostream& os, const EPoint& point)
{
    os << "      vertex   " << point(0) << "     "
                            << point(1) << "     "
                            << point(2) << "\n";
    return os;
}

::std::ostream& operator<<(::std::ostream& os, const VectorOfPoints& facet);
::std::ostream& operator<<(::std::ostream& os, const VectorOfPoints& facet)
{
    os << "  facet normal   0.00000  0.00000  0.00000\n"
       << "    outer loop\n";
    for (size_t i = 0 ; i < facet.size() ; ++i)
    {
        os << facet[i];
    }
    os << "    endloop\n"
       << "  endfacet\n";
    return os;
}

std::string write_stl(const VectorOfVectorOfPoints& mesh)
{
    std::stringstream data;
    data << "solid MYSOLID\n";
    for (size_t i = 0 ; i < mesh.size() ; ++i)
    {
        data << mesh[i];
    }
    data << "endsolid MYSOLID\n";
    return data.str();
}

::std::ostream& operator<<(::std::ostream& os, const VectorOfVectorOfPoints& mesh)
{
    os << write_stl(mesh);
    return os;
}

std::string write_stl(const VectorOfPoints& mesh)
{
    return write_stl(VectorOfVectorOfPoints(1,mesh));
}

Eigen::Vector3d unit_normal__(const Matrix3x& points);
Eigen::Vector3d unit_normal__(const Matrix3x& points)
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

void write_binary_stl(const VectorOfVectorOfPoints& stl, std::ostream& os)
{
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
            for (size_t k = 0 ; k < 3 ; ++k) M((int)k,(int)j) = stl[i][j]((int)k);
        }
        const Eigen::Vector3d normal = unit_normal__(M);
        float x = (float)normal(0);
        float y = (float)normal(1);
        float z = (float)normal(2);
        os.write(reinterpret_cast<const char*>(&x), sizeof(float));
        os.write(reinterpret_cast<const char*>(&y), sizeof(float));
        os.write(reinterpret_cast<const char*>(&z), sizeof(float));
        for (int j = 0 ; j < 3 ; ++j)
        {
            float x = (float)M(0,j);
            float y = (float)M(1,j);
            float z = (float)M(2,j);
            os.write(reinterpret_cast<const char*>(&x), sizeof(float));
            os.write(reinterpret_cast<const char*>(&y), sizeof(float));
            os.write(reinterpret_cast<const char*>(&z), sizeof(float));
        }
        os.write(reinterpret_cast<const char*>(&spacer), sizeof(uint16));
    }
}
