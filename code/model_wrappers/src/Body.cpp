/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "YamlBody.hpp"

Body::Body() : name(""),
G(ssc::kinematics::Point()),
m(0),
mesh(MeshPtr()),
total_inertia(MatrixPtr()),
solid_body_inertia(MatrixPtr()),
inverse_of_the_total_inertia(MatrixPtr()),
x_relative_to_mesh(0),
y_relative_to_mesh(0),
z_relative_to_mesh(0),
mesh_to_body(ssc::kinematics::RotationMatrix()),
M(PointMatrixPtr ()),
u(0),
v(0),
w(0),
p(0),
q(0),
r(0)
{

}
