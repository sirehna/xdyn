/*
 * FixedFrame.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef FIXEDFRAME_HPP_
#define FIXEDFRAME_HPP_

#include "AbstractFrame.hpp"
#include <tr1/memory>

/** \author cec
 *  \brief The (unique) fixed frame of reference.
 *  \details All other frames (indirectly) point to this one. This is a singleton.
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/FixedFrameTest.cpp FixedFrameTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/FixedFrameTest.cpp FixedFrameTest expected output
 */
namespace kinematics {
class FixedFrame : public AbstractFrame
{
    private:
        FixedFrame();

    public:
        static std::tr1::shared_ptr<FixedFrame> get_instance();
};
}
#endif /* FIXEDFRAME_HPP_ */
