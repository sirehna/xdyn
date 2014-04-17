/*
 * Transform.hpp
 *
 *  Created on: 8 avr. 2014
 *      Author: maroff
 */

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "Point.hpp"
#include "PointMatrix.hpp"
#include "RotationMatrix.hpp"
#include "Velocity.hpp"

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
            Transform(const Point& translation, const RotationMatrix& rotation, const std::string& to_frame);
            Transform(const Point& translation, const std::string& to_frame);
            Transform(const RotationMatrix& rotation, const std::string& from_frame, const std::string& to_frame);
            Point operator*(const Point& P) const;
            PointMatrix operator*(const PointMatrix& P) const;
            Velocity operator*(const Velocity& P) const;
            Transform operator*(const Transform& P) const;
            std::string get_from_frame() const;
            std::string get_to_frame() const;

        private:
            Transform();
            Point t;
            RotationMatrix r;
            std::string to_frame;
    };
}
#endif /* TRANSFORM_HPP_ */
