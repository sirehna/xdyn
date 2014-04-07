#include "Vector3DInFrame.hpp"

using namespace kinematics;

Vector3DInFrame::Vector3DInFrame(const FramePtr& ref, const double x, const double y, const double z) : rw::math::Vector3D<double>(x,y,z), reference_frame(ref)
{
}

Vector3DInFrame::Vector3DInFrame(const FramePtr& ref, const rw::math::Vector3D<double>& v) : rw::math::Vector3D<double>(v), reference_frame(ref)
{
}

Vector3DInFrame Vector3DInFrame::operator/(const FramePtr& frame) const
{
    const rw::math::Transform3D<> T = reference_frame->get_transformation_from_this_to_frame(frame);
    return Vector3DInFrame(frame, T*(*this));
}

FramePtr Vector3DInFrame::get_frame() const
{
    return reference_frame;
}
