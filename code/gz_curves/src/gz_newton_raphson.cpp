/*
 * gz_newton_raphson.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */

#include "InternalErrorException.hpp"

#include "gz_newton_raphson.hpp"

double delta(const Eigen::Vector3d& X1, const Eigen::Vector3d& X2);
double delta(const Eigen::Vector3d& X1, const Eigen::Vector3d& X2)
{
    return (X1-X2).array().abs().maxCoeff();
}

GZ::State GZ::newton_raphson(const GZ::State& X0,     //!< Initial value
                             const GZ::FType& f,      //!< Function calculating the sum of forces
                             const GZ::KComputer& K,  //!< Function calculating the stiffness matrix (f')
                             const size_t max_it,     //!< Maximum number of iterations
                             const double eps         //!< Desired precision
                              )
{

    Eigen::Vector3d Xn = X0;
    Eigen::Vector3d Xn_1 = X0.array() + 2*eps;
    for (size_t i = 0 ; i < max_it ; ++i)
    {
        if (delta(Xn,Xn_1) < eps) return Xn;
        Xn_1 = Xn;
        const auto k = K(Xn_1);
        const double det_k = k(0,0)*k(1,1) - k(0,1)*k(1,0);
        if (std::abs(det_k) < 1E-15)
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "Determinant of stiffness matrix is very close to zero (|det K| = " << std::abs(det_k) << " < 1E-15)");
        }
        const auto y = f(Xn_1).state;
        if (delta(y,Eigen::Vector3d::Zero()) < eps) return Xn;
        Xn(0) = Xn_1(0) - (k(1,1)*y(0) - k(0,1)*y(2))/det_k;
        Xn(2) = Xn_1(2) - (k(0,0)*y(2) - k(1,0)*y(0))/det_k;
    }
    return Xn;
}
