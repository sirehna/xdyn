/*
 * yaml2eigen.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "yaml2eigen.hpp"

#include "InvalidInputException.hpp"
#include "YamlAngle.hpp"
#include "YamlCoordinates.hpp"
#include "YamlPoint.hpp"
#include "YamlPosition.hpp"
#include "YamlRotation.hpp"

#include <ssc/exception_handling.hpp>

ssc::kinematics::RotationMatrix angle2matrix(const YamlAngle& a, const YamlRotation& rotations)
{
    const ssc::kinematics::EulerAngles e(a.phi, a.theta, a.psi);

    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "z", "y'", "x''"))
            return ssc::kinematics::rotation_matrix<ssc::kinematics::INTRINSIC,
                                                    ssc::kinematics::CHANGING_ANGLE_ORDER,
                                                    3, 2, 1>(e);
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In YAML file, section 'rotations convention', convention [" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "] is not currently supported.");
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "In YAML file, section 'rotations convention', ordering rotations by '" << rotations.order_by << "' is not currently supported");
    }
    return ssc::kinematics::RotationMatrix();
}

bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third)
{
    return (convention.at(0) == first) and (convention.at(1) == second) and (convention.at(2) == third);
}

ssc::kinematics::Point make_point(const YamlCoordinates& point, const std::string& frame)
{
    return ssc::kinematics::Point(frame,
                                  point.x,
                                  point.y,
                                  point.z);
}

ssc::kinematics::Point make_point(const YamlPoint& point)
{
    return make_point(point, point.frame);
}

ssc::kinematics::Transform make_transform(const YamlPosition& p, const std::string& to_frame, const YamlRotation& rotations)
{
    const ssc::kinematics::Point translation = make_point(p.coordinates, p.frame);
    const ssc::kinematics::RotationMatrix rotation = angle2matrix(p.angle, rotations);
    return ssc::kinematics::Transform(translation, rotation, to_frame);
}
