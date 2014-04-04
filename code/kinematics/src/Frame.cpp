#include "Frame.hpp"

using namespace kinematics;

FramePtr Frame::make_new(const Vector3DInFrame& P, const rw::math::Rotation3D<double>& R)
{
    return FramePtr(new Frame(P,R));
}

FramePtr Frame::make_new(const FramePtr& F, const rw::math::Vector3D<double>& P, const rw::math::Rotation3D<double>& R)
{
    return make_new(Vector3DInFrame(F,P), R);
}

FramePtr Frame::make_new(const Vector3DInFrame& P)
{
    return FramePtr(new Frame(P,rw::math::Rotation3D<double>()));
}

FramePtr Frame::make_new(const FramePtr& F, const rw::math::Rotation3D<double>& R)
{
    return FramePtr(new Frame(Vector3DInFrame(F,0,0,0),R));
}

rw::math::Transform3D<double> Frame::get_transformation_from_this_to(const FramePtr& destination_frame //!< The destination frame
                                              )
{
    const typename std::map<FramePtr, rw::math::Transform3D<double> >::const_iterator it  = transformations.find(destination_frame);
    if (it == transformations.end())
    {
        transformations[destination_frame];
    }
    return transformations[destination_frame];
}

        Frame::Frame(const Vector3DInFrame& position_of_origin, const rw::math::Rotation3D<double>& rotation) : P(position_of_origin),
                                                                                             R(rotation),
                                                                                             transformations(std::map<FramePtr, rw::math::Transform3D<double> >())
        {}
