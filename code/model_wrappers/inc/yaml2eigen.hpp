/*
 * yaml2eigen.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef YAML2EIGEN_HPP_
#define YAML2EIGEN_HPP_

#include <string>
#include <vector>

#include <ssc/kinematics.hpp>

struct YamlAngle;
struct YamlCoordinates;
struct YamlPoint;
struct YamlPosition;
struct YamlRotation;

/** \details Converts (phi,theta,psi) to a rotation matrix, taking the
 *           rotation convention into account
 */
ssc::kinematics::RotationMatrix angle2matrix(const YamlAngle& a, const YamlRotation& rotations);

/** \brief Utility function used by angle2matrix
 *  \returns True if 'convention' == [first,second,third]
 */
bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third);

ssc::kinematics::Point make_point(const YamlPoint& point);
ssc::kinematics::Point make_point(const YamlCoordinates& point, const std::string& frame);
ssc::kinematics::Transform make_transform(const YamlPosition& position_of_frame_relative_to_another, const std::string& to_frame, const YamlRotation& rotations);

#endif /* YAML2EIGEN_HPP_ */
