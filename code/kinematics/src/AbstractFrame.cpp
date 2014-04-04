#include "AbstractFrame.hpp"

using namespace kinematics;

AbstractFrame::AbstractFrame() : R(rw::math::Rotation3D<double>())
{
}

AbstractFrame::~AbstractFrame()
{
}
