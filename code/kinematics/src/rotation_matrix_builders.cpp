#include "rotation_matrix_builders.hpp"
#include "EulerAngles.hpp"
#include <Eigen/Geometry>

typedef Eigen::Matrix<double, 3, 1> Vector;

RotationMatrix kinematics::rot(const double lambda1, const double lambda2, const double lambda3, const double beta)
{
    return Eigen::AngleAxis<double>(beta, Vector(lambda1, lambda2, lambda3)).toRotationMatrix();
}

namespace kinematics
{
    template <> RotationMatrix rotation_matrix<INTRINSIC, ORDERED_BY_ANGLE, CARDAN, 3, 2, 1>(const EulerAngles& angles)
    {
        const RotationMatrix Rx_phi   = rot(1,0,0, angles.phi);
        const RotationMatrix Ry_theta = rot(0,1,0, angles.theta);
        const RotationMatrix Rz_psi   = rot(0,0,1, angles.psi);
        return Rz_psi*Ry_theta*Rx_phi;
    }
}
