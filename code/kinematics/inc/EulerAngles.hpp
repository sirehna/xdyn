/*
 * EulerAngles.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef EULERANGLES_HPP_
#define EULERANGLES_HPP_

/** \author cec
 *  \brief 
 *  \details 
 *  \ingroup 
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/EulerAnglesTest.cpp EulerAnglesTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/EulerAnglesTest.cpp EulerAnglesTest expected output
 */
class EulerAngles
{
    public:
        EulerAngles();
        EulerAngles(const double phi, const double theta, const double psi);
        double phi;
        double theta;
        double psi;
};

#endif /* EULERANGLES_HPP_ */
