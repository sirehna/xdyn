/*
 * Velocity.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef VELOCITY_HPP_
#define VELOCITY_HPP_

#include "Vector3DInFrame.hpp"

/** \author cec
 *  \brief Translation & rotational speed of a point in a reference frame
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/VelocityTest.cpp VelocityTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/VelocityTest.cpp VelocityTest expected output
 */
namespace kinematics {
class Velocity
{
    public:
        Velocity(const Vector3DInFrame& point_in_frame, const FramePtr& ref_frame_, const FramePtr& projection_frame_);
        void set_projection_frame(const FramePtr& projection_frame_);

        double u;
        double v;
        double w;
        double p;
        double q;
        double r;

    private:
        Velocity();
        Vector3DInFrame P;
        FramePtr ref_frame;
        FramePtr projection_frame;

};
}
#endif /* VELOCITY_HPP_ */
