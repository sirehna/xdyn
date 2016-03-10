#ifndef GEOMETRICTYPES3D_HPP_
#define GEOMETRICTYPES3D_HPP_

#include <vector>

#include <Eigen/Dense>

typedef Eigen::Vector3d EPoint;
typedef std::vector<EPoint> VectorOfPoints;
typedef std::vector<VectorOfPoints> VectorOfVectorOfPoints;
typedef Eigen::Matrix<double,3,Eigen::Dynamic> Matrix3x;

#endif
