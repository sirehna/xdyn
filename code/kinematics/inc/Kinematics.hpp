/*
 * Kinematics.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef KINEMATICS_HPP_
#define KINEMATICS_HPP_

#include <string>
#include "rotation_matrix_builders.hpp"
#include "Point.hpp"
#include <tr1/memory>

/** \author cec
 *  \brief Interface to robwork
 *  \details
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/KinematicsTest.cpp KinematicsTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/KinematicsTest.cpp KinematicsTest expected output
 */
class Kinematics
{
    public:
        Kinematics();
        void add_frame(const std::string& frame_of_reference, const Point& P, const RotationMatrix& R);
        Point project(const Point& P, const std::string& destination_frame);

    private:
        class Impl;
        std::tr1::shared_ptr<Impl> pimpl;
};

#endif /* KINEMATICS_HPP_ */
