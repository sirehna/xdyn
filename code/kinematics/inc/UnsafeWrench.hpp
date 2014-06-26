/*
 * UnsafeWrench.hpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#ifndef UNSAFEWRENCH_HPP_
#define UNSAFEWRENCH_HPP_

#include "Wrench.hpp"

/** \author cec
 *  \date May 19, 2014, 2:18:28 PM
 *  \brief Just like a Wrench, except reference frames aren't checked (faster).
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/UnsafeWrenchTest.cpp UnsafeWrenchTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/UnsafeWrenchTest.cpp UnsafeWrenchTest expected output
 */
class UnsafeWrench : public Wrench
{
    public:
        UnsafeWrench();
        UnsafeWrench(const Point& P);
        UnsafeWrench(const Point& P, const Eigen::Vector3d& force, const Eigen::Vector3d& torque);
        UnsafeWrench(const UnsafeWrench& rhs);
        UnsafeWrench(const Wrench& rhs);

        UnsafeWrench operator+(const UnsafeWrench& rhs) const;
        UnsafeWrench operator-(const UnsafeWrench& rhs) const;
        UnsafeWrench& operator+=(const UnsafeWrench& rhs);
        UnsafeWrench& operator-=(const UnsafeWrench& rhs);
        UnsafeWrench& operator+=(const Wrench& rhs);
        UnsafeWrench& operator+=(const Wrench&& rhs);
        UnsafeWrench& operator-=(const Wrench& rhs);
};

#endif /* UNSAFEWRENCH_HPP_ */
