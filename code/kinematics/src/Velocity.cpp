#include "Velocity.hpp"

using namespace kinematics;

Velocity::Velocity(const Vector3DInFrame& point_in_frame, const FramePtr& ref_frame_, const FramePtr& projection_frame_) : u(0),v(0),w(0),p(0),q(0),r(0), P(point_in_frame), ref_frame(ref_frame_), projection_frame(projection_frame_)  {}

void Velocity::set_projection_frame(const FramePtr& projection_frame_)
{
    projection_frame = projection_frame_;
}
