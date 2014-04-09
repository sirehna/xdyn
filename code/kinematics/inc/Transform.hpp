/*
 * Transform.hpp
 *
 *  Created on: 8 avr. 2014
 *      Author: maroff
 */

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "Point.hpp"
#include "RotationMatrix.hpp"

/** \author cec
 *  \brief 
 *  \details 
 *  \ingroup 
 *  \section ex1 Example
 *  \snippet _________/unit_tests/src/TransformTest.cpp TransformTest example
 *  \section ex2 Expected output
 *  \snippet _________/unit_tests/src/TransformTest.cpp TransformTest expected output
 */
namespace kinematics
{
    class Transform
    {
        public:
            Transform();
            Transform(const Point& translation, const RotationMatrix& rotation);
            Transform(const Point& translation);
            Transform(const RotationMatrix& rotation);
            Point operator*(const Point& P) const;
            Transform operator*(const Transform& P) const;

        private:
            Point t;
            RotationMatrix r;
    };
}
#endif /* TRANSFORM_HPP_ */
