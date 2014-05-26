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

#include <ostream>

/** \author cec
 *  \brief Can express positions, speeds & wrenches in a different frame of reference.
 *  \details
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/TransformTest.cpp TransformTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/TransformTest.cpp TransformTest expected output
 */
namespace kinematics
{
    class Transform
    {
        public:
            Transform();
            Transform(const Point& translation, const RotationMatrix& rotation, const std::string& to_frame);
            Transform(const Point& translation, const std::string& to_frame);
            Transform(const RotationMatrix& rotation, const std::string& from_frame, const std::string& to_frame);
            Point operator*(const Point& P) const;
            PointMatrix operator*(const PointMatrix& P) const;
            Velocity operator*(const Velocity& P) const;
            Transform operator*(const Transform& P) const;
            std::string get_from_frame() const;
            std::string get_to_frame() const;
            Transform inverse() const;
            Point get_point() const;
            RotationMatrix get_rot() const;


            friend std::ostream& operator<<(std::ostream& os, const Transform& T);

        private:
            Point t;
            RotationMatrix r;
            std::string to_frame;
    };
    std::ostream& operator<<(std::ostream& os, const Transform& T);
    Transform identity(const std::string& frame);
}


#endif /* TRANSFORM_HPP_ */
