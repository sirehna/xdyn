/*
 * coriolis_and_centripetal.hpp
 *
 *  Created on: Jun 3, 2014
 *      Author: cady
 */

#ifndef CORIOLIS_AND_CENTRIPETAL_HPP_
#define CORIOLIS_AND_CENTRIPETAL_HPP_

#include <Eigen/Dense>
#include "Wrench.hpp"

typedef Eigen::Matrix<double,6,6> Matrix6x6;
typedef Eigen::Matrix<double,3,3> Matrix3x3;

class Point;

/**  \author cec
  *  \date Jun 4, 2014, 8:36:11 AM
  *  \brief Computes the Coriolis & centripetal forces.
  *  \details These are the forces due to the fact that the equation of Newton's
  *  second law of motion is written in a non-Gallilean frame: hence its speed
  *  must be taken into account. The inertia matrix provided is the
  *  rigid body inertia. It should, in general, not be the added mass matrix or
  *  the sum of the added mass matrix & the rigid-body inertia matrix because
  *  the added mass forces are experimentally found not to be proportional to
  *  the acceleration but they are proportional to the derivative of the projection
  *  in the body frame of the linear velocity of the body with respect to the
  *  (Gallilean) earth frame.
  *  Reference: Guidance And Control of Ocean Vehicles ISBN 0 471 94113 1
                Thor I. Fossen, Hydrodynamic Forces And Moments equation (2.125)
                chapter 2.4.1 page 36 C_A(\nu)
  *
  *  \returns Coriolis wrench (force + torque)
  *  \snippet kinematics/unit_tests/src/coriolis_and_centripetalTest.cpp coriolis_and_centripetalTest coriolis_and_centripetal_example
  */
Wrench coriolis_and_centripetal(const Point& P,              //!< Point & frame of expression of the resulting Coriolis wrench
                                const Matrix6x6& inertia,    //!< Rigid-body inertia matrix, given at P, in P's frame of reference
                                const Eigen::Vector3d& v,    //!< Translation speed of point P relative to the (Gallilean) NED (Nort-East-Down) Earth frame [u,v,w]
                                const Eigen::Vector3d& omega //!< Rotation vector of the (non-Gallilean) body frame, relative to the NED frame [p,q,r]
                                );

#endif /* CORIOLIS_AND_CENTRIPETAL_HPP_ */
