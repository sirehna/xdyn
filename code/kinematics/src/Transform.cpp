#include "Transform.hpp"

using namespace kinematics;

Transform::Transform() : t(Point()), r(RotationMatrix(Eigen::MatrixXd::Identity(3,3)))
{
}

Transform::Transform(const Point& translation) : t(translation), r(RotationMatrix(Eigen::MatrixXd::Identity(3,3)))
{
}

Transform::Transform(const RotationMatrix& rotation) : t(Point()), r(rotation)
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
