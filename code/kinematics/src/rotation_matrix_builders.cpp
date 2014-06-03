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

    template <> RotationMatrix rotation_matrix<INTRINSIC, CHANGING_ANGLE_ORDER, CARDAN, 1, 2, 3>(const EulerAngles& angles)
    {
        const RotationMatrix Rx_phi   = rot(1,0,0, angles.phi);
        const RotationMatrix Ry_theta = rot(0,1,0, angles.theta);
        const RotationMatrix Rz_psi   = rot(0,0,1, angles.psi);
        return RotationMatrix(Rx_phi*Ry_theta*Rz_psi);
    }

    template <> RotationMatrix rotation_matrix<INTRINSIC, CHANGING_ANGLE_ORDER, CARDAN, 3, 2, 1>(const EulerAngles& angles)
    {
        const RotationMatrix Rx_phi   = rot(1,0,0, angles.phi);
        const RotationMatrix Ry_theta = rot(0,1,0, angles.theta);
        const RotationMatrix Rz_psi   = rot(0,0,1, angles.psi);
        return RotationMatrix(Rz_psi*Ry_theta*Rx_phi);
    }

    template <> RotationMatrix rotation_matrix<EXTRINSIC, CHANGING_ANGLE_ORDER, CARDAN, 3, 2, 1>(const EulerAngles& angles)
    {
        const RotationMatrix Rx_phi   = rot(1,0,0, angles.phi);
        const RotationMatrix Ry_theta = rot(0,1,0, angles.theta);
        const RotationMatrix Rz_psi   = rot(0,0,1, angles.psi);
        return RotationMatrix(Rx_phi*Ry_theta*Rz_psi);
    }
}
