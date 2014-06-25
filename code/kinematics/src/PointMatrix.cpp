#include "PointMatrix.hpp"
#include "Point.hpp"

PointMatrix::PointMatrix() : m(Matrix3Xd()), frame("")
{
}

PointMatrix::PointMatrix(PointMatrix&& other) : m(Matrix3Xd()), frame("")
{
    swap(other);
}

PointMatrix::PointMatrix(const std::string& frame_, const size_t nb_of_columns) : m(Matrix3Xd()), frame(frame_)
{
    m.resize(3,(int)nb_of_columns);
}

PointMatrix::PointMatrix(const Matrix3Xd& m_, const std::string& frame_) : m(m_), frame(frame_)
{
}

PointMatrix::PointMatrix(const PointMatrix& other) : m(other.m), frame(other.frame)
{
}

PointMatrix& PointMatrix::operator=(PointMatrix other)
{
    swap(other);
    return *this;
}

void PointMatrix::swap(PointMatrix& other)
{
    m.swap(other.m);
    frame.swap(other.frame);
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

void swap(PointMatrix& one, PointMatrix& the_other)
{
    one.swap(the_other);
}
