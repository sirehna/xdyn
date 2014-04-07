/*
 * FixedFrame.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef FIXEDFRAME_HPP_
#define FIXEDFRAME_HPP_

#include "AbstractFrame.hpp"


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
    public:
        static std::tr1::shared_ptr<FixedFrame> get_instance();
        rw::math::Transform3D<double> get_transformation_from_parent_to_this() const;
        rw::math::Transform3D<double> get_transformation_from_this_to_frame(const FramePtr& from) const;

    protected:
        bool is_base_frame() const;

    private:
        void add_frame_to_tree(AbstractFrame* parent_frame_, const FramePtr& child_frame);
        rw::math::Transform3D<double> get_transformation_from_frame_to_frame(const std::string& from, const FramePtr& to) const;
        FixedFrame();
        class Impl;
        std::tr1::shared_ptr<Impl> pimpl;
};
}
#endif /* FIXEDFRAME_HPP_ */
