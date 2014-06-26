/*
 * transform_double_equal.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "Kinematics.hpp"
#include "almost_equal.hpp"
#include "transform_double_equal.hpp"
#include "Transform.hpp"

#include <iostream>

bool double_equal(const kinematics::Transform& t1, const kinematics::Transform& t2, const double eps)
{
    if (t1.get_from_frame() != t2.get_from_frame())
    {
        std::cerr << "'from' frames don't match:" << std::endl;
        std::cerr << "t1 : " << t1.get_from_frame() << " -> " << t1.get_to_frame() << std::endl;
        std::cerr << "t2 : " << t2.get_from_frame() << " -> " << t2.get_to_frame() << std::endl;
        return false;
    }
    if (t1.get_to_frame() != t2.get_to_frame())
    {
        std::cerr << "'to' frames don't match." << std::endl;
        std::cerr << "t1 : " << t1.get_from_frame() << " -> " << t1.get_to_frame() << std::endl;
        std::cerr << "t2 : " << t2.get_from_frame() << " -> " << t2.get_to_frame() << std::endl;
        return false;
    }
    const Point u1(t1.get_from_frame(), 1, 0, 0);
    const Point u2(t1.get_from_frame(), 0, 1, 0);
    const Point u3(t1.get_from_frame(), 0, 0, 1);

    const Point u11 = t1*u1;
    const Point u12 = t1*u2;
    const Point u13 = t1*u3;
    const Point u21 = t2*u1;
    const Point u22 = t2*u2;
    const Point u23 = t2*u3;

    const std::vector<double> std_u11(&u11.v[0], &u11.v[0] + 3);
    const std::vector<double> std_u12(&u12.v[0], &u12.v[0] + 3);
    const std::vector<double> std_u13(&u13.v[0], &u13.v[0] + 3);
    const std::vector<double> std_u21(&u21.v[0], &u21.v[0] + 3);
    const std::vector<double> std_u22(&u22.v[0], &u22.v[0] + 3);
    const std::vector<double> std_u23(&u23.v[0], &u23.v[0] + 3);

    if (not(almost_equal(std_u11, std_u21, eps)))
    {
        std::cerr << "t1*(1,0,0) = " << u11.v.transpose() << std::endl;
        std::cerr << "t2*(1,0,0) = " << u21.v.transpose() << std::endl;
        return false;
    }

    if (not(almost_equal(std_u12, std_u22, eps)))
    {
        std::cerr << "t1*(0,1,0) = " << u12.v.transpose() << std::endl;
        std::cerr << "t2*(0,1,0) = " << u22.v.transpose() << std::endl;
        return false;
    }
    if (not(almost_equal(std_u13, std_u23, eps)))
    {
        std::cerr << "t1*(0,0,1) = " << u13.v.transpose() << std::endl;
        std::cerr << "t2*(0,0,1) = " << u23.v.transpose() << std::endl;
        return false;
    }

    return true;
}
