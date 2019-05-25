/*
 * convert_quaternion_to_euler.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: cady
 */

#include <ssc/check_ssc_version.hpp>
#include <ssc/kinematics.hpp>
#include <iostream>

CHECK_SSC_VERSION(8,0)

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define RAD2DEG (180./PI)

ssc::kinematics::EulerAngles convert(const double qr, const double qi, const double qj, const double qk);
ssc::kinematics::EulerAngles convert(const double qr, const double qi, const double qj, const double qk)
{
    using namespace ssc::kinematics;
    const Eigen::Quaternion<double> quat(qr,qi,qj,qk);
    const RotationMatrix R = quat.toRotationMatrix();
    return ssc::kinematics::euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R);
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << std::string(argv[0]) << " converts a quaternion to this Euler angle in degree"<< std::endl;
        std::cout << "Euler convention is \"INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1\"" << std::endl;
        return 0;
    }
    else if (argc != 5)
    {
        std::cerr << "Need exactly 4 arguments (qr, qi, qj, qk): received " << argc-1 << std::endl;
        return 1;
    }

    const double qr = std::atof(argv[1]);
    const double qi = std::atof(argv[2]);
    const double qj = std::atof(argv[3]);
    const double qk = std::atof(argv[4]);

    const ssc::kinematics::EulerAngles angles = convert(qr, qi, qj, qk);

    std::cout << "phi: "   << angles.phi*RAD2DEG << std::endl
              << "theta: " << angles.theta*RAD2DEG << std::endl
              << "psi: "   << angles.psi*RAD2DEG << std::endl;
    return 0;
}

