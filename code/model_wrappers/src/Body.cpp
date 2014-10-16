/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "YamlBody.hpp"

Body::Body() : name(),
G(),
m(),
mesh(),
total_inertia(),
solid_body_inertia(),
inverse_of_the_total_inertia(),
x_relative_to_mesh(),
y_relative_to_mesh(),
z_relative_to_mesh(),
mesh_to_body(),
M(),
u(),
v(),
w(),
p(),
q(),
r(),
intersector(),
down_direction_in_mesh_frame(),
absolute_surface_elevation()
{

}
