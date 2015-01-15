/*
 * BodyStates.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */

#include "BodyStates.hpp"
#include "EnvironmentAndFrames.hpp"
#include "StateMacros.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlBody.hpp"
#include "yaml2eigen.hpp"

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
x(),
y(),
z(),
phi(),
theta(),
psi(),
u(),
v(),
w(),
p(),
q(),
r(),
qr(),
qi(),
qj(),
qk(),
intersector(),
down_direction_in_mesh_frame(),
hydrodynamic_forces_calculation_point()
{
}


ssc::kinematics::EulerAngles BodyStates::convert(const ssc::kinematics::RotationMatrix& R, const YamlRotation& rotations) const
{
    using namespace ssc::kinematics;
    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "z", "y'", "x''"))
            return euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R);
        std::stringstream ss;
        ss << "Rotation convention '" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "' is not currently supported.";
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, ss.str());
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, std::string("Ordering rotations by '") + rotations.order_by + "' is not currently supported");
    }
    return EulerAngles();
}

ssc::kinematics::EulerAngles BodyStates::get_angles(const YamlRotation& c) const
{
    return convert(get_rot_from_ned_to_body(),c);
}

ssc::kinematics::EulerAngles BodyStates::get_angles(const StateType& all_states, const size_t idx, const YamlRotation& c) const
{
    return convert(get_rot_from_ned_to(all_states,idx),c);
}

ssc::kinematics::RotationMatrix BodyStates::get_rot_from_ned_to_body() const
{
    return Eigen::Quaternion<double>(qr,qi,qj,qk).matrix();
}

ssc::kinematics::RotationMatrix BodyStates::get_rot_from_ned_to(const StateType& x, const size_t idx) const
{
    return Eigen::Quaternion<double>(*_QR(x,idx),*_QI(x,idx),*_QJ(x,idx),*_QK(x,idx)).matrix();
}
