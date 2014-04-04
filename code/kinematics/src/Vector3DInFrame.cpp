#include "Vector3DInFrame.hpp"

using namespace kinematics;

Vector3DInFrame::Vector3DInFrame(const FramePtr& ref, const double x, const double y, const double z) : rw::math::Vector3D<double>(x,y,z), reference_frame(ref)
{
}

Vector3DInFrame::Vector3DInFrame(const FramePtr& ref, const rw::math::Vector3D<double>& v) : rw::math::Vector3D<double>(v), reference_frame(ref)
{
}

Vector3DInFrame Vector3DInFrame::operator/(const FramePtr&) const
{
    return *this;
}
