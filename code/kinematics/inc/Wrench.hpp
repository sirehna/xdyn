/*
 * Wrench.hpp
 *
 *  Created on: 24 avr. 2014
 *      Author: cady
 */

#ifndef WRENCH_HPP_
#define WRENCH_HPP_

#include "Point.hpp"

namespace kinematics
{
    class Transform;
}

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
        Wrench();
        Wrench(const Point& P);
        Wrench(const Point& P, const Eigen::Vector3d& force, const Eigen::Vector3d& torque);
        Wrench(const Wrench& rhs);
        Wrench& operator=(const Wrench& rhs);

        Eigen::Vector3d force;
        Eigen::Vector3d torque;
        double& X;
        double& Y;
        double& Z;
        double& K;
        double& M;
        double& N;

        Wrench operator+(const Wrench& rhs) const;
        Wrench operator-(const Wrench& rhs) const;

        /**  \author cec
         *  \date Apr 25, 2014, 9:34:06 AM
         *  \brief Transports the wrench to a new point in the same frame.
         *  \details Only translation, no rotation involved.
         *  \returns Wrench at new point.
         *  \snippet kinematics/unit_tests/src/WrenchTest.cpp WrenchTest change_point_of_application_example
         */
        Wrench change_point_of_application(const Point& Q //!< New point of application
                ) const;

        /**  \author cec
         *  \date Apr 25, 2014, 9:34:06 AM
         *  \brief Expresses the wrench in a new frame, without changing the point of application
         *  \details Only rotation, no translation involved.
         *  \returns Wrench at same point, but expressed in a new frame.
         *  \snippet kinematics/unit_tests/src/WrenchTest.cpp WrenchTest change_point_of_application_example
         */
        Wrench change_frame_but_keep_ref_point(const kinematics::Transform& T //!< Transform from current frame to new frame
                ) const;

        /**  \author cec
         *  \date Apr 25, 2014, 11:01:49 AM
         *  \brief Expresses the wrench in a new frame, using the new origin as point of application
         *  \details Rotation + translation. Please note that the order is not important, as
         *  RT + AB x RF = RT + RF x BA = R (T + F x BA) = R (T + AB x F)
         *  \returns Wrench expressed in a new frame & moved to a new point.
         *  \snippet kinematics/unit_tests/src/WrenchTest.cpp WrenchTest project_and_change_ref_example
         */
        Wrench change_ref_point_then_change_frame(const kinematics::Transform& T //!< Transform from current frame to new frame
                ) const;

    private:
        Point P; // Point of application
};

#endif /* WRENCH_HPP_ */
