/*
 * gz_newton_raphson.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */

#ifndef GZ_NEWTON_RAPHSON_HPP_
#define GZ_NEWTON_RAPHSON_HPP_

#include <functional>

#include <Eigen/Dense>

#include "GZTypes.hpp"

namespace GZ
{
    typedef std::function<Resultant(const ::GZ::State&)> FType;
    typedef std::function<Eigen::Matrix2d(const Eigen::Vector3d&)> KComputer;

    ::GZ::State newton_raphson(const ::GZ::State& X0, //!< Initial value (in particular, phi=X0(1) is imposed)
                               const FType& f,        //!< Function calculating the sum of forces
                               const KComputer& K,    //!< Function calculating the stiffness matrix (f') for z & theta but not phi as it is imposed
                               const size_t max_it,   //!< Maximum number of iterations. Computation stops when that number of steps is used
                               const double eps       //!< Desired precision: algorithm stops when max(X_old(0)-X_new(0),X_old(1)-X_new(1),X_old(2)-X_new(2))<eps
                              );
}

#endif /* GZ_NEWTON_RAPHSON_HPP_ */
