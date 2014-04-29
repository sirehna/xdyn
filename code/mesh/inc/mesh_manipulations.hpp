/*
 * mesh_manipulations.hpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#ifndef MESH_MANIPULATIONS_HPP_
#define MESH_MANIPULATIONS_HPP_

#include <Eigen/Dense>
#include <vector>

Eigen::Vector3d normal(const std::vector<Eigen::Vector3d>& polygon);
Eigen::Vector3d unit_normal(const std::vector<Eigen::Vector3d>& polygon);
Eigen::Vector3d barycenter(const std::vector<Eigen::Vector3d>& polygon);
double area(const std::vector<Eigen::Vector3d>& polygon);


#endif /* MESH_MANIPULATIONS_HPP_ */
