#include "rotation_matrix_builders.hpp"
#include <Eigen/Geometry>


typedef Eigen::Matrix<double, 3, 1> Vector;

RotationMatrix kinematics::rot(const double lambda1, const double lambda2, const double lambda3, const double beta)
{
    return Eigen::AngleAxis<double>(beta, Vector(lambda1, lambda2, lambda3)).toRotationMatrix();
}
