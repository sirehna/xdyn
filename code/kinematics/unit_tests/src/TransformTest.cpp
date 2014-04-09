/*
 * TransformTest.cpp
 *
 *  Created on: 8 avr. 2014
 *      Author: maroff
 */

#include "TransformTest.hpp"
#include "Transform.hpp"
#include "Point.hpp"
#include "rotation_matrix_builders.hpp"
#include "test_macros.hpp"
#include "extra_test_assertions.hpp"
#include "KinematicsException.hpp"

#include <cmath>

#define PI (4.*atan(1.))
#define EPS 1E-13

TransformTest::TransformTest() : a(DataGenerator(1215))
{
}

TransformTest::~TransformTest()
{
}

void TransformTest::SetUp()
{
}

void TransformTest::TearDown()
{
}

template <> RotationMatrix TypedScalarDataGenerator<RotationMatrix>::get() const
{
    double lambda1 = random<double>().between(-1,1);
    double lambda2 = random<double>().between(-1,1);
    double lambda3 = random<double>().between(-1,1);
    const double D = sqrt(lambda1*lambda1+lambda2*lambda2+lambda3*lambda3);
    lambda1 = lambda1/D;
    lambda2 = lambda2/D;
    lambda3 = lambda3/D;
    const double beta = random<double>().between(-PI,PI);
    return kinematics::rot(lambda1, lambda2, lambda3, beta);
}

template <> Point TypedScalarDataGenerator<Point>::get() const
{
    const double x = random<double>().between(-10,10);
    const double y = random<double>().between(-10,10);
    const double z = random<double>().between(-10,10);
    return Point(random<std::string>(), x, y, z);
}

Point TransformTest::random_point_in_frame(const std::string& frame) const
{
    const double x = a.random<double>().between(-10,10);
    const double y = a.random<double>().between(-10,10);
    const double z = a.random<double>().between(-10,10);
    return Point(frame, x, y, z);
}

Point TransformTest::random_point() const
{
    return random_point_in_frame(a.random<std::string>());
}

TEST_F(TransformTest, can_translate_a_point)
{
//! [TransformTest example]
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const Point P1 = random_point();
        const Point P2 = random_point_in_frame(P1.get_frame());
        kinematics::Transform T(P1, a.random<std::string>());
        const Point Q = T*P2;

        ASSERT_SMALL_RELATIVE_ERROR(P1.x+P2.x,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P1.y+P2.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P1.z+P2.z,Q.z,EPS);
    }
//! [TransformTest example]
//! [TransformTest expected output]
//! [TransformTest expected output]
}

TEST_F(TransformTest, can_compose_two_translations)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const std::string middle_frame = a.random<std::string>();
        const Point P1 = random_point();
        const Point P2 = random_point_in_frame(middle_frame);
        kinematics::Transform T1(P1,middle_frame);
        kinematics::Transform T2(P2,a.random<std::string>());
        const Point Q = T2*T1*P1;

        ASSERT_SMALL_RELATIVE_ERROR(2*P1.x+P2.x,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(2*P1.y+P2.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(2*P1.z+P2.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_rotate_a_point)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double beta = a.random<double>().between(-PI,PI);
        RotationMatrix R = kinematics::rot(0,0,1, beta);
        const Point P = random_point();
        kinematics::Transform T(R, P.get_frame(), a.random<std::string>());
        const Point Q = T*P;

        ASSERT_SMALL_RELATIVE_ERROR(cos(beta)*P.x-sin(beta)*P.y,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(sin(beta)*P.x+cos(beta)*P.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_compose_two_rotations)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double beta1 = a.random<double>().between(-PI,PI);
        const double beta2 = a.random<double>().between(-PI,PI);
        RotationMatrix R1 = kinematics::rot(0,0,1, beta1);
        RotationMatrix R2 = kinematics::rot(0,0,1, beta2);
        const Point P = random_point();
        const std::string middle_frame = a.random<std::string>();
        kinematics::Transform T1(R1, P.get_frame(), middle_frame);
        kinematics::Transform T2(R2, middle_frame, a.random<std::string>());
        const Point Q = T2*T1*P;

        ASSERT_SMALL_RELATIVE_ERROR(cos(beta1+beta2)*P.x-sin(beta1+beta2)*P.y,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(sin(beta1+beta2)*P.x+cos(beta1+beta2)*P.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_translate_and_rotate_a_point)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const Point O = random_point();
        const Point P = random_point_in_frame(O.get_frame());
        const double beta = a.random<double>().between(-PI,PI);
        RotationMatrix R = kinematics::rot(0,0,1, beta);
        kinematics::Transform T(O,R,a.random<std::string>());
        const Point Q = T*P;

        ASSERT_SMALL_RELATIVE_ERROR(O.x+cos(beta)*P.x-sin(beta)*P.y,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(O.y+sin(beta)*P.x+cos(beta)*P.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(O.z+P.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, should_throw_if_applying_transform_to_a_point_in_wrong_frame)
{
    for (size_t i = 0 ; i<1000 ; ++i)
    {
        const std::string F1 = a.random<std::string>();
        const std::string F2 = a.random<std::string>();
        const Point P1 = random_point_in_frame(F1);
        const Point P2 = random_point_in_frame(F2);
        RotationMatrix R = a.random<RotationMatrix>();
        kinematics::Transform T1(P1,R,a.random<std::string>());
        kinematics::Transform T2(P2,R,a.random<std::string>());
        ASSERT_NO_THROW(T1*P1);
        ASSERT_THROW(T1*P2, KinematicsException);
        ASSERT_NO_THROW(T2*P2);
        ASSERT_THROW(T2*P1, KinematicsException);
    }
}

TEST_F(TransformTest, should_throw_if_composing_transforms_in_wrong_frame)
{
    for (size_t i = 0 ; i<1000 ; ++i)
    {
        const std::string F1 = a.random<std::string>();
        const std::string F2 = a.random<std::string>();
        const Point O1 = random_point_in_frame(F1);
        const Point O2 = random_point_in_frame(F2);
        RotationMatrix R = a.random<RotationMatrix>();
        kinematics::Transform T1(O1,R,F2);
        kinematics::Transform T2(O2,R,a.random<std::string>());
        ASSERT_NO_THROW(T2*T1);
        ASSERT_THROW(T1*T1, KinematicsException);
        ASSERT_THROW(T1*T2, KinematicsException);
        ASSERT_THROW(T2*T2, KinematicsException);
    }
}
