#ifndef TRIMESHTESTDATA_HPP
#define TRIMESHTESTDATA_HPP

#include "GeometricTypes3d.hpp"

VectorOfPoints one_triangle();
VectorOfPoints degenerated_triangle();
VectorOfVectorOfPoints unit_cube();
VectorOfVectorOfPoints two_triangles();
VectorOfVectorOfPoints two_triangles_immerged();
VectorOfVectorOfPoints trapezium();
VectorOfVectorOfPoints n_gone(const size_t n);
VectorOfVectorOfPoints cube(const double a, const double x0, const double y0, const double z0);
VectorOfVectorOfPoints tetrahedron(const double a, const double x0, const double y0, const double z0);

#endif
