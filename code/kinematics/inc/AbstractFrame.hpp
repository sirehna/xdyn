/*
 * AbstractFrame.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef ABSTRACTFRAME_HPP_
#define ABSTRACTFRAME_HPP_

#include <rw/math/Rotation3D.hpp>
#include <tr1/memory>

/** \author cec
 *  \brief Provides a common base class for FixedFrame & Frame
 *  \ingroup kinematics
 */
namespace kinematics {
class AbstractFrame
{
    public:
        AbstractFrame();
        virtual ~AbstractFrame();

    protected:
        rw::math::Rotation3D<double> R;
};
typedef std::tr1::shared_ptr<AbstractFrame> FramePtr;
}
#endif /* ABSTRACTFRAME_HPP_ */
