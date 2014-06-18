/*
 * BodyBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "MeshBuilder.hpp"
#include "PointMatrix.hpp"
#include "YamlBody.hpp"

BodyBuilder::BodyBuilder(const YamlRotation& convention) : rotations(convention)
{
}

Body BodyBuilder::build(const YamlBody& input, const VectorOfVectorOfPoints& mesh) const
{
    Body ret;
    ret.name = input.name;
    ret.G = Point(input.dynamics.centre_of_inertia.frame,
                  input.dynamics.centre_of_inertia.x,
                  input.dynamics.centre_of_inertia.y,
                  input.dynamics.centre_of_inertia.z);
    ret.m = input.dynamics.mass;
    ret.x_relative_to_mesh = input.position_of_body_frame_relative_to_mesh.coordinates.x;
    ret.y_relative_to_mesh = input.position_of_body_frame_relative_to_mesh.coordinates.y;
    ret.z_relative_to_mesh = input.position_of_body_frame_relative_to_mesh.coordinates.z;
    ret.mesh = MeshPtr(new Mesh(MeshBuilder(mesh).build()));
    ret.M = PointMatrixPtr(new PointMatrix(ret.mesh->nodes, ret.name));
    return ret;
}
