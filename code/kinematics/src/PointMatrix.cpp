#include "PointMatrix.hpp"

PointMatrix::PointMatrix(const std::string& frame_) : m(Matrix3Xd()), frame(frame_)
{
}

PointMatrix::PointMatrix(const Matrix3Xd& m_, const std::string& frame_) : m(m_), frame(frame_)
{
}

std::string PointMatrix::get_frame() const
{
    return frame;
}
