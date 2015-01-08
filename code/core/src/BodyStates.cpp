/*
 * BodyStates.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */

#include "BodyStates.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlBody.hpp"

BodyStates::BodyStates() : name(),
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
hydrodynamic_forces_calculation_point()
{
}

