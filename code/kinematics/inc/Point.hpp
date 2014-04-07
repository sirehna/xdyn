/*
 * Point.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef POINT_HPP_
#define POINT_HPP_

#include <string>

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
        Point(const std::string& frame, const double x, const double y, const double z);
        double x;
        double y;
        double z;
        std::string frame;

    private:
        Point();
};

#endif /* POINT_HPP_ */
