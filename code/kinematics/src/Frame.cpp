#include "Frame.hpp"
#include "FrameException.hpp"
#include <rw/math/Vector3D.hpp>
#include <rw/common/Exception.hpp>

using namespace kinematics;

void Frame::add_frame_to_tree(AbstractFrame* parent_frame_, const std::tr1::shared_ptr<AbstractFrame>& child_frame)
{
    if (not(get_parent()))
    {
        THROW(__PRETTY_FUNCTION__, FrameException, std::string("No parent frame for frame '") + get_name() + "'");
    }
    get_parent()->add_frame_to_tree(parent_frame_, child_frame);
}

rw::math::Transform3D<double> Frame::get_transformation_from_frame_to_frame(const std::string& from, const FramePtr& to) const
{
    if (not(get_parent()))
    {
        THROW(__PRETTY_FUNCTION__, FrameException, std::string("No parent frame for frame '") + get_name() + "'");
    }
    return get_parent()->get_transformation_from_frame_to_frame(from, to);
}

rw::math::Transform3D<double> Frame::get_transformation_from_this_to_frame(const FramePtr& to) const
{
    return get_transformation_from_frame_to_frame(name, to);
}

FramePtr Frame::make_new(const Vector3DInFrame& P, const rw::math::Rotation3D<double>& R, const std::string& name)
{
    FramePtr new_frame(new Frame(P,R,name));
    FramePtr base = FixedFrame::get_instance();
    base->add_frame_to_tree(P.get_frame().get(), new_frame);
    return new_frame;
}

FramePtr Frame::make_new(FramePtr& parent_frame_, const rw::math::Vector3D<double>& P, const rw::math::Rotation3D<double>& R, const std::string& name)
{
    return make_new(Vector3DInFrame(parent_frame_,P), R, name);
}

FramePtr Frame::make_new(const Vector3DInFrame& P, const std::string& name)
{
    if (name.empty())
    {
        THROW(__PRETTY_FUNCTION__, FrameException, std::string("Attempted to create a frame with an empty name"));
    }
    if (name == "base")
    {
        THROW(__PRETTY_FUNCTION__, FrameException, "'base' is not a valid frame name");
    }
    FramePtr new_frame(new Frame(P,rw::math::Rotation3D<double>(),name));
    FramePtr base = FixedFrame::get_instance();
    try
    {
        base->add_frame_to_tree(P.get_frame().get(), new_frame);
    }
    catch(rw::common::Exception&)
    {
        THROW(__PRETTY_FUNCTION__, FrameException, std::string("A frame named '" + name + "' already exists"));
    }
    return new_frame;
}

FramePtr Frame::make_new(FramePtr& parent_frame_, const rw::math::Rotation3D<double>& R, const std::string& name)
{
    FramePtr new_frame(new Frame(Vector3DInFrame(parent_frame_,0,0,0),R,name));
    FramePtr base = FixedFrame::get_instance();
    base->add_frame_to_tree(parent_frame_.get(), new_frame);
    return new_frame;
}

rw::math::Transform3D<double> Frame::get_transformation_from_parent_to_this() const
{
    return rw::math::Transform3D<double>(P, R);
}

Frame::Frame(const Vector3DInFrame& position_of_origin, const rw::math::Rotation3D<double>& rotation, const std::string& frame_name) :
               AbstractFrame(position_of_origin.get_frame().get(), rotation, frame_name),
               P(position_of_origin)
{
}
