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
    typedef std::function<Eigen::Matrix3d(const Eigen::Vector3d&)> KComputer;

    ::GZ::State newton_raphson(const ::GZ::State& X0, //!< Initial value
                               const FType& f,        //!< Function calculating the sum of forces
                               const KComputer& K,    //!< Function calculating the stiffness matrix (f')
                               const size_t max_it,   //!< Maximum number of iterations
                               const double eps       //!< Desired precision
                              );
}

#endif /* GZ_NEWTON_RAPHSON_HPP_ */
