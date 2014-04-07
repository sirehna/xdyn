#include "AbstractFrame.hpp"

using namespace kinematics;


AbstractFrame::AbstractFrame(AbstractFrame* parent, const rw::math::Rotation3D<double>& R_, const std::string& frame_name) : R(R_), parent_frame(parent), name(frame_name)
{
}

AbstractFrame::~AbstractFrame()
{
}

std::string AbstractFrame::get_name() const
{
    return name;
}

bool AbstractFrame::is_base_frame() const
{
    return false;
}

AbstractFrame* AbstractFrame::get_parent() const
{
    return parent_frame;
}

AbstractFrame::AbstractFrame(const AbstractFrame& rhs) : R(rhs.R), parent_frame(rhs.parent_frame), name(rhs.name)
{
}

AbstractFrame& AbstractFrame::operator=(const AbstractFrame& rhs)
{
    if (this != &rhs)
    {
        R = rhs.R;
        parent_frame = rhs.parent_frame;
        name = rhs.name;
    }
    return *this;
}
