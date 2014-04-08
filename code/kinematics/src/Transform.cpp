#include "Transform.hpp"

using namespace kinematics;

Transform::Transform() : t(Point()), r(RotationMatrix())
{
}


Transform::Transform(const Point& translation, const RotationMatrix& rotation) : t(translation), r(rotation)
{
}

Point Transform::operator*(const Point& P) const
{
    return Point(P.v+t.v);
}

Transform Transform::operator*(const Transform& P) const
{
    return Transform(Point(P.t.v+t.v), P.r);
}
