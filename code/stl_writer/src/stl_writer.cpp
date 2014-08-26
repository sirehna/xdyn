#include <sstream>

#include "stl_writer.hpp"

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
