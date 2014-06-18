/*
 * BodyBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "BodyBuilderException.hpp"
#include "EulerAngles.hpp"
#include "MeshBuilder.hpp"
#include "PointMatrix.hpp"
#include "rotation_matrix_builders.hpp"
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
    ret.mesh_to_body = angle2matrix(input.position_of_body_frame_relative_to_mesh.angle);
    return ret;
}

RotationMatrix BodyBuilder::angle2matrix(const YamlAngle& a) const
{
    const EulerAngles e(a.phi, a.theta, a.psi);

    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "z", "y'", "x''"))
            return kinematics::rotation_matrix<kinematics::INTRINSIC,
                                               kinematics::CHANGING_ANGLE_ORDER,
                                               kinematics::CARDAN, 3, 2, 1>(e);
        std::stringstream ss;
        ss << "Rotation convention '" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "' is not currently supported.";
        THROW(__PRETTY_FUNCTION__, BodyBuilderException, ss.str());
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, BodyBuilderException, std::string("Ordering rotations by '") + rotations.order_by + "' is not currently supported");
    }
    return RotationMatrix();
}

bool BodyBuilder::match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third) const
{
    return (convention.at(0) == first) and (convention.at(1) == second) and (convention.at(2) == third);
}
