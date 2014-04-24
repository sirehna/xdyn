/*
 * Wrench.hpp
 *
 *  Created on: 24 avr. 2014
 *      Author: cady
 */

#ifndef WRENCH_HPP_
#define WRENCH_HPP_

#include <string>
#include "Point.hpp"

/** \author cec
 *  \date 24 avr. 2014, 12:35:30
 *  \brief Stores a force & a torque, projected in a given reference frame.
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/WrenchTest.cpp WrenchTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/WrenchTest.cpp WrenchTest expected output
 */
class Wrench
{
    public:
        Wrench(const std::string& frame);
        double X;
        double Y;
        double Z;
        double K;
        double M;
        double N;

    private:
        Wrench(); // Disabled because we must specify a frame for the coordinates
        std::string frame;
        Point P; // Point of application
};

#endif /* WRENCH_HPP_ */
