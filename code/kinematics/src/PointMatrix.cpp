#include "PointMatrix.hpp"
#include "Point.hpp"

PointMatrix::PointMatrix() : m(Matrix3Xd()), frame("")
{
}

PointMatrix::PointMatrix(const std::string& frame_, const size_t nb_of_columns) : m(Matrix3Xd()), frame(frame_)
{
    m.resize(3,nb_of_columns);
}

PointMatrix::PointMatrix(const Matrix3Xd& m_, const std::string& frame_) : m(m_), frame(frame_)
{
}

std::string PointMatrix::get_frame() const
{
    return frame;
}

PointMatrix PointMatrix::operator+(const Point& P) const
{
    const Matrix3Xd M = m.colwise() + P.v;
    return PointMatrix(M, frame);
}

PointMatrix operator+(const Point& P, const PointMatrix& M)
{
    return M+P;
}
