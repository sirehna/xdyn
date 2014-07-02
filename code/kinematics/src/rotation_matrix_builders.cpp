#include "rotation_matrix_builders.hpp"
#include "EulerAngles.hpp"
#include <Eigen/Geometry>

typedef Eigen::Matrix<double, 3, 1> Vector;

#define SQUARE(x) (x*x)

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

    template <> EulerAngles euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, CARDAN, 3, 2, 1>(const RotationMatrix& R)
    {
        EulerAngles ret;
        // By default, R is column-major: as we want a Coordinate Transform Matrix
        // and not a rotation matrix, we need to transpose it: therefore, we're
        // reading the matrix as if it were row-major (even though it's column-
        // major).
        const double* ctm = R.data();
        const double eps = 1e-13;
        const double hyp = sqrt(SQUARE(ctm[0])+SQUARE(ctm[1]));

        ret.theta = atan2(-ctm[2],hyp);

        if(hyp > eps) // Not degenerated
        {
            ret.phi = atan2(ctm[5],ctm[8]);
            ret.psi = atan2(ctm[1],ctm[0]);
        }
        else
        {
            ret.phi = 0;
            ret.psi = atan2(-ctm[3],ctm[4]);
        }
        return ret;
    }
}
