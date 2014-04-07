#include "Kinematics.hpp"

void Kinematics::add_frame(const std::string&, const Point& , const RotationMatrix& )
{

}

Point Kinematics::project(const Point& , const std::string& frame)
{
    return Point(frame, 0, 0, 0);
}
