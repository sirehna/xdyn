#ifndef POINTMATRIX_HPP_
#define POINTMATRIX_HPP_

#include <string>
#include <Eigen/Dense>

typedef Eigen::Matrix<double,3,Eigen::Dynamic> Matrix3Xd;

class Point;
class PointMatrix
{
    public:
        PointMatrix();
        PointMatrix(const std::string& frame, const size_t nb_of_columns);
        PointMatrix(const Matrix3Xd& m,const std::string& frame);
        PointMatrix(const PointMatrix& other);
        PointMatrix(PointMatrix&& other);
        PointMatrix& operator=(PointMatrix other);
        Matrix3Xd m;
        std::string get_frame() const;
        PointMatrix operator+(const Point& P) const;
        void swap(PointMatrix& other);

    private:
        std::string frame;
};

PointMatrix operator+(const Point& P, const PointMatrix& M);

void swap(PointMatrix& one, PointMatrix& the_other);

#endif
