/*
 * mesh_manipulations.hpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#ifndef MESH_MANIPULATIONS_HPP_
#define MESH_MANIPULATIONS_HPP_

#include "GeometricTypes3d.hpp"

Eigen::Vector3d normal(const VectorOfPoints& polygon);
Eigen::Vector3d unit_normal(const VectorOfPoints& polygon);

Eigen::Vector3d barycenter(const VectorOfPoints& points);
double area(const VectorOfPoints& polygon);


#endif /* MESH_MANIPULATIONS_HPP_ */
