/*
 * Point.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef POINT1_HPP_
#define POINT1_HPP_

#include <Eigen/Dense>

/** \author cec
 *  \brief 
 *  \details 
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/PointTest.cpp PointTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/PointTest.cpp PointTest expected output
 */
class Point
{
    public:
        Point();
        Point(const Point& P);
        Point(const Eigen::Vector3d& v);
        Point(const double x, const double y, const double z);
        Point& operator=(const Point& rhs);

    public:
        Eigen::Vector3d v;
        double& x;
        double& y;
        double& z;
};

#endif /* POINT1_HPP_ */
