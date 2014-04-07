/*
 * Frame.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef FRAME_HPP_
#define FRAME_HPP_

#include <map>
#include <string>
#include <rw/models/WorkCell.hpp>

#include "Vector3DInFrame.hpp"
#include <rw/math/Rotation3D.hpp>
#include "FixedFrame.hpp"

/** \author cec
 *  \brief Reference frame.
 *  \details Always has a parent frame.
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/FrameTest.cpp FrameTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/FrameTest.cpp FrameTest expected output
 */
namespace kinematics {
class Frame : public AbstractFrame
{
    public:
        static FramePtr make_new(const Vector3DInFrame& P, const rw::math::Rotation3D<double>& R, const std::string& name);
        static FramePtr make_new(FramePtr& F, const rw::math::Vector3D<double>& P, const rw::math::Rotation3D<double>& R, const std::string& name);
        static FramePtr make_new(const Vector3DInFrame& P, const std::string& name);
        static FramePtr make_new(FramePtr& F, const rw::math::Rotation3D<double>& R, const std::string& name);

        rw::math::Transform3D<double> get_transformation_from_this_to_frame(const FramePtr& from) const;

        /** \author cec
         *  \date 2 avr. 2014, 09:54:06
         *  \brief Returns the transformation from this to another frame
         *  \returns A transformation
         *  \snippet kinematics/unit_tests/src/FrameTest.cpp Frame get_transformation_from_this_to_example
         */
        rw::math::Transform3D<double> get_transformation_from_parent_to_this() const;
    private:
        Frame(const Vector3DInFrame& position_of_origin, const rw::math::Rotation3D<double>& rotation, const std::string& frame_name);
        Frame();
        void add_frame_to_tree(AbstractFrame* parent_frame_, const std::tr1::shared_ptr<AbstractFrame>& child_frame);
        rw::math::Transform3D<double> get_transformation_from_frame_to_frame(const std::string& from, const FramePtr& to) const;


        Vector3DInFrame P;
};

}

//std::tr1::shared_ptr<kinematics::AbstractFrame> kinematics::Frame<T>::fixed_frame = std::tr1::shared_ptr<kinematics::AbstractFrame<T> >(new FixedFrame<T>());
#endif /* FRAME_HPP_ */
