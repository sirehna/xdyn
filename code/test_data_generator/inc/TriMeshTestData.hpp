#ifndef TRIMESHTESTDATA_HPP
#define TRIMESHTESTDATA_HPP

#include "GeometricTypes3d.hpp"

VectorOfPoints one_triangle();
VectorOfPoints one_triangle_clockwise();
VectorOfPoints degenerated_triangle();
VectorOfVectorOfPoints unit_cube();
VectorOfVectorOfPoints unit_cube_with_incorrect_orientation();
VectorOfVectorOfPoints unit_cube_clockwise();
VectorOfVectorOfPoints two_triangles();
VectorOfVectorOfPoints two_triangles_immerged();
VectorOfVectorOfPoints trapezium();
VectorOfVectorOfPoints n_gone(const size_t n);
VectorOfVectorOfPoints cube(const double a, const double x0, const double y0, const double z0, const bool close=true);
VectorOfVectorOfPoints tetrahedron(const double a, const double x0, const double y0, const double z0);
VectorOfVectorOfPoints tetrahedron_clockwise(const double a, const double x0, const double y0, const double z0);
VectorOfVectorOfPoints generated_stl();
VectorOfVectorOfPoints L();
VectorOfVectorOfPoints U();

#endif
