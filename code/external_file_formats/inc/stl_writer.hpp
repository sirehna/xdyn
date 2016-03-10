#ifndef STL_WRITER_HPP_
#define STL_WRITER_HPP_

#include <ostream>
#include <string>

#include "GeometricTypes3d.hpp"

std::string write_stl(const VectorOfPoints& mesh);
std::string write_stl(const VectorOfVectorOfPoints& mesh);
::std::ostream& operator<<(::std::ostream& os, const VectorOfVectorOfPoints& mesh);

/**  \brief Writes a mesh to output stream, in binary STL format
  */
void write_binary_stl(const VectorOfVectorOfPoints& stl, std::ostream& os);

#endif /* STL_WRITER_HPP_ */
