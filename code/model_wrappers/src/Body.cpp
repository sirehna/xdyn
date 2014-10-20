/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SurfaceElevationInterface.hpp"
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
hydrodynamic_forces_calculation_point()
{
}

void Body::update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                    const double t)
{
    if (env.w.use_count())
    {
        env.w->update_surface_elevation(M, env.k,t);
        const std::vector<double> dz = env.w->get_relative_wave_height();
        intersector->update_intersection_with_free_surface(env.w->get_relative_wave_height(),
                                                           env.w->get_surface_elevation());
    }
}

