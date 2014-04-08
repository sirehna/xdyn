#include "Point.hpp"

Point::Point() : v(0,0,0), x(v[0]), y(v[1]), z(v[2])
{

}

Point::Point(const Eigen::Vector3d& v_) : v(v_), x(v[0]), y(v[1]), z(v[2])
{
}

Point::Point(const Point& P) : v(P.v), x(v[0]), y(v[1]), z(v[2])
{
}

Point::Point(const double x_, const double y_, const double z_) : v(x_,y_,z_), x(v[0]), y(v[1]), z(v[2])
{
}

Point& Point::operator=(const Point& rhs)
{
    if (&rhs != this)
    {
        v = rhs.v;
        x = v[0];
        y = v[1];
        z = v[2];
    }
    return *this;
}
