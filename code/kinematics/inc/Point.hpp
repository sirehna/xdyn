/*
 * Point.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef POINT1_HPP_
#define POINT1_HPP_

#include <Eigen/Dense>
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
        Point(const std::string& frame);
        Point(const Point& P);
        Point(const std::string& frame, const Eigen::Vector3d& v);
        Point(const std::string& frame, const double x, const double y, const double z);
        Point& operator=(const Point& rhs);

        Eigen::Vector3d v;
        double& x;
        double& y;
        double& z;
        std::string get_frame() const;
        
    private:
        Point();
        std::string frame;
};

#endif /* POINT1_HPP_ */
