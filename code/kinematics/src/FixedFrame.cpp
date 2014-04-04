#include "FixedFrame.hpp"

using namespace kinematics;

FixedFrame::FixedFrame()
{}

std::tr1::shared_ptr<FixedFrame> FixedFrame::get_instance()
{
    static std::tr1::shared_ptr<FixedFrame> instance;
    if (not(instance.get()))
    {
        instance = std::tr1::shared_ptr<FixedFrame>(new FixedFrame());
    }
    return instance;
}
