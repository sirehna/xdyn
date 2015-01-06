#ifndef SURFACEELEVATIONGRID_HPP_
#define SURFACEELEVATIONGRID_HPP_

#include <Eigen/Core>

struct SurfaceElevationGrid
{
    double t;
    Eigen::VectorXd x;
    Eigen::VectorXd y;
    Eigen::MatrixXd z;
    SurfaceElevationGrid():t(0.0),x(),y(),z(){};
};

#endif /* SURFACEELEVATIONGRID_HPP_ */
