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

BodyStates::BodyStates(const double Tmax) : AbstractStates<History>(Tmax),
name(),
G(),
mesh(),
total_inertia(),
solid_body_inertia(),
inverse_of_the_total_inertia(),
x_relative_to_mesh(),
y_relative_to_mesh(),
z_relative_to_mesh(),
mesh_to_body(),
M(),
intersector(),
g_in_mesh_frame(),
hydrodynamic_forces_calculation_point(),
convention()
{
}

BodyStates& BodyStates::operator=(const AbstractStates<History>& rhs)
{
    AbstractStates<History>::operator=(rhs);
    return *this;
}

ssc::kinematics::EulerAngles BodyStates::convert(const ssc::kinematics::RotationMatrix& R, const YamlRotation& rotations)
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

std::tuple<double,double,double,double> BodyStates::convert(const ssc::kinematics::EulerAngles& R, const YamlRotation& rotations)
{
    using namespace ssc::kinematics;
    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "z", "y'", "x''"))
        {
            Eigen::Quaternion<double> quat(rotation_matrix<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R));
            std::tuple<double,double,double,double> ret;
            std::get<0>(ret) = quat.w();
            std::get<1>(ret) = quat.x();
            std::get<2>(ret) = quat.y();
            std::get<3>(ret) = quat.z();
            return ret;
        }

        std::stringstream ss;
        ss << "Rotation convention '" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "' is not currently supported.";
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, ss.str());
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, std::string("Ordering rotations by '") + rotations.order_by + "' is not currently supported");
    }
    return std::make_tuple(0.,0.,0.,0.);
}

ssc::kinematics::EulerAngles BodyStates::get_angles() const
{
    return convert(get_rot_from_ned_to_body(),convention);
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
    return Eigen::Quaternion<double>(qr(),qi(),qj(),qk()).matrix();
}

ssc::kinematics::RotationMatrix BodyStates::get_rot_from_ned_to(const StateType& x, const size_t idx) const
{
    return Eigen::Quaternion<double>(*_QR(x,idx),*_QI(x,idx),*_QJ(x,idx),*_QK(x,idx)).matrix();
}

std::vector<double> BodyStates::get_current_state_values(const size_t idx) const
{
    std::vector<double> s(13, 0);
    s[XIDX(idx)] = x();
    s[YIDX(idx)] = y();
    s[ZIDX(idx)] = z();
    s[UIDX(idx)] = u();
    s[VIDX(idx)] = v();
    s[WIDX(idx)] = w();
    s[PIDX(idx)] = p();
    s[QIDX(idx)] = q();
    s[RIDX(idx)] = r();
    s[QRIDX(idx)] = qr();
    s[QIIDX(idx)] = qi();
    s[QJIDX(idx)] = qj();
    s[QKIDX(idx)] = qk();
    return s;
}
