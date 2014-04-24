#ifndef POINTMATRIX_HPP_
#define POINTMATRIX_HPP_

#include <string>
#include <Eigen/Dense>

typedef Eigen::Matrix<double,3,Eigen::Dynamic> Matrix3Xd;

class Point;
class PointMatrix
{
    public:
        PointMatrix(const std::string& frame, const size_t nb_of_columns);
        PointMatrix(const Matrix3Xd& m,const std::string& frame);
        Matrix3Xd m;
        std::string get_frame() const;
        PointMatrix operator+(const Point& P) const;

    private:
        PointMatrix();
        std::string frame;
};

PointMatrix operator+(const Point& P, const PointMatrix& M);

#endif
