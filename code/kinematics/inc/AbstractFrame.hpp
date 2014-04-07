/*
 * AbstractFrame.hpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#ifndef ABSTRACTFRAME_HPP_
#define ABSTRACTFRAME_HPP_

#include <rw/math/Rotation3D.hpp>
#include <rw/math/Transform3D.hpp>
#include <tr1/memory>
#include <string>

/** \author cec
 *  \brief Provides a common base class for FixedFrame & Frame
 *  \ingroup kinematics
 */
namespace kinematics {

class AbstractFrame
{
    public:
        AbstractFrame(AbstractFrame* parent, const rw::math::Rotation3D<double>& R_, const std::string& frame_name);
        virtual ~AbstractFrame();
        AbstractFrame(const AbstractFrame& rhs);
        AbstractFrame& operator=(const AbstractFrame& rhs);
        std::string get_name() const;
        virtual rw::math::Transform3D<double> get_transformation_from_parent_to_this() const = 0;
        virtual rw::math::Transform3D<double> get_transformation_from_this_to_frame(const std::tr1::shared_ptr<AbstractFrame>& from) const = 0;
        AbstractFrame* get_parent() const;
        virtual bool is_base_frame() const;
        virtual void add_frame_to_tree(AbstractFrame* parent_frame, const std::tr1::shared_ptr<AbstractFrame>& child_frame) = 0;
        virtual rw::math::Transform3D<double> get_transformation_from_frame_to_frame(const std::string& from, const std::tr1::shared_ptr<AbstractFrame>& to) const = 0;

    protected:
        rw::math::Rotation3D<double> R;
        AbstractFrame* parent_frame;
        std::string name;

    private:
        AbstractFrame();
};
typedef std::tr1::shared_ptr<AbstractFrame> FramePtr;
}
#endif /* ABSTRACTFRAME_HPP_ */
