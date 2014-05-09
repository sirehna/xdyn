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
template <typename T>  T sum(const std::vector<T>& points)
{
    typename std::vector<T>::const_iterator it = points.begin();
    T zero;
    zero = zero-zero;
    T compensation = zero;
    T sum = zero;
    for (;it!=points.end() ; ++it)
    {
        const T myTmp1 = *it - compensation;
        const T myTmp2 = sum + myTmp1;
        compensation = (myTmp2 - sum) - myTmp1;
        sum = myTmp2;
    }
    return sum;
}

Eigen::Vector3d barycenter(const VectorOfPoints& points);
double area(const VectorOfPoints& polygon);


#endif /* MESH_MANIPULATIONS_HPP_ */
