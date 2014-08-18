/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "YamlBody.hpp"

Body::Body() : name(""),
G(Point()),
m(0),
mesh(MeshPtr()),
total_inertia(MatrixPtr()),
solid_body_inertia(MatrixPtr()),
inverse_of_the_total_inertia(MatrixPtr()),
x_relative_to_mesh(0),
y_relative_to_mesh(0),
z_relative_to_mesh(0),
mesh_to_body(RotationMatrix()),
M(PointMatrixPtr ())
{

}
