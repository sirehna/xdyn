/*
 * Position.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef POSITION_HPP_
#define POSITION_HPP_

#include <tr1/memory>
#include "AbstractFrame.hpp"
#include <rw/math/Vector3D.hpp>

/** \author cec
 *  \brief Position of a point in a reference frame.
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/PositionTest.cpp PositionTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/PositionTest.cpp PositionTest expected output
 */
namespace kinematics {
class Vector3DInFrame : public rw::math::Vector3D<double>
{
    public:
        Vector3DInFrame(const FramePtr& ref, const double x, const double y, const double z);

        Vector3DInFrame(const FramePtr& ref, const rw::math::Vector3D<double>& v);

        /** \author cec
         *  \date 2 avr. 2014, 09:49:53
         *  \brief Projects a point in a frame
         *  \details Each point (Vector3DInFrame) is attached to a specific frame. This method creates another object representing the same point, seen from another frame. That other frame will be attached to the returned point.
         *  \returns Vector3D<T>
         *  \snippet kinematics/unit_tests/src/Vector3DInFrameTest.cpp Vector3DInFrame operator/_example
         */
        Vector3DInFrame operator/(const FramePtr&  //!< Frame in which we want to project the coordinates.
                             ) const;

    private:
        Vector3DInFrame();
        FramePtr reference_frame;
};
}

#endif /* POSITION_HPP_ */
