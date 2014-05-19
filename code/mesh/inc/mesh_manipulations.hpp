/*
 * mesh_manipulations.hpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#ifndef MESH_MANIPULATIONS_HPP_
#define MESH_MANIPULATIONS_HPP_

#include "GeometricTypes3d.hpp"

Eigen::Vector3d normal(const Matrix3x& polygon);
Eigen::Vector3d unit_normal(const Matrix3x& polygon);

Eigen::Vector3d barycenter(const Matrix3x& points);
double area(const Matrix3x& polygon);


#endif /* MESH_MANIPULATIONS_HPP_ */
