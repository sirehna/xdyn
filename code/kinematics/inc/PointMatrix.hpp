#ifndef POINTMATRIX_HPP_
#define POINTMATRIX_HPP_

#include <string>
#include "GeometricTypes3d.hpp"

class PointMatrix
{
    public:
        PointMatrix(const std::string& frame);
        PointMatrix(const Matrix3Xd& m,const std::string& frame);
        Matrix3Xd m;
        std::string get_frame() const;
    private:
        PointMatrix();
        std::string frame;
};

#endif
