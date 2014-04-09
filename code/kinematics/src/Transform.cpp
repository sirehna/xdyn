#include "Transform.hpp"
#include "KinematicsException.hpp"

using namespace kinematics;

Transform::Transform(const Point& translation, const std::string& to_frame_) : t(translation), r(RotationMatrix(Eigen::MatrixXd::Identity(3,3))), to_frame(to_frame_)
{
}

Transform::Transform(const RotationMatrix& rotation, const std::string& from_frame, const std::string& to_frame_) : t(Point(from_frame)), r(rotation), to_frame(to_frame_)
{
}

Transform::Transform(const Point& translation, const RotationMatrix& rotation, const std::string& to_frame_) : t(translation), r(rotation), to_frame(to_frame_)
{
}

Point Transform::operator*(const Point& P) const
{
    if (P.get_frame() != get_from_frame())
    {
        THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Frames don't match: transform goes from ") + get_from_frame() + " to " + to_frame + ", but point lies in " + P.get_frame());
    }
    return Point(to_frame, r*P.v+t.v);
}

Transform Transform::operator*(const Transform& T) const
{
    return Transform(Point(T.get_from_frame(), T.t.v+t.v), r*T.r, to_frame);
}

std::string Transform::get_from_frame() const
{
    return t.get_frame();
}