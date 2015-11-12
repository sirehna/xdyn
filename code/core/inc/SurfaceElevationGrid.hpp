#ifndef SURFACEELEVATIONGRID_HPP_
#define SURFACEELEVATIONGRID_HPP_

#include <Eigen/Core>

struct SurfaceElevationGrid
{
    double t;
    Eigen::VectorXd x;
    Eigen::VectorXd y;
    Eigen::MatrixXd z;

    SurfaceElevationGrid(
            const double t_ = 0.0,
            const Eigen::VectorXd& x_ = Eigen::VectorXd(),
            const Eigen::VectorXd& y_ = Eigen::VectorXd(),
            const Eigen::MatrixXd& z_ = Eigen::MatrixXd()
            ):t(t_),x(x_),y(y_),z(z_){};

    SurfaceElevationGrid(
            const std::size_t nx,
            const std::size_t ny,
            const double t_ = 0.0
            ):t(t_),x(Eigen::VectorXd((long)nx)),
              y(Eigen::VectorXd((long)ny)),z(Eigen::MatrixXd((long)nx,(long)ny)){};
};

#endif /* SURFACEELEVATIONGRID_HPP_ */
