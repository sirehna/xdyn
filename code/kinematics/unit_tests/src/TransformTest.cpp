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
    return Point(x, y, z);
}

TEST_F(TransformTest, can_translate_a_point)
{
//! [TransformTest example]
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const Point P1 = a.random<Point>();
        const Point P2 = a.random<Point>();
        kinematics::Transform T(P1);
        const Point Q = T*P2;

        ASSERT_DOUBLE_EQ(P1.x+P2.x,Q.x);
        ASSERT_DOUBLE_EQ(P1.y+P2.y,Q.y);
        ASSERT_DOUBLE_EQ(P1.z+P2.z,Q.z);
    }
//! [TransformTest example]
//! [TransformTest expected output]
//! [TransformTest expected output]
}

TEST_F(TransformTest, can_compose_two_translations)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const Point P1 = a.random<Point>();
        const Point P2 = a.random<Point>();
        kinematics::Transform T1(P1);
        kinematics::Transform T2(P2);
        const Point Q = T1*T2*P1;

        ASSERT_SMALL_RELATIVE_ERROR(2*P1.x+P2.x,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(2*P1.y+P2.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(2*P1.z+P2.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_only_use_a_transformation_on_a_point_if_the_frames_match)
{

}

TEST_F(TransformTest, can_only_compose_transformations_if_their_frames_match)
{

}

TEST_F(TransformTest, can_rotate_a_point)
{
    kinematics::rot(0, 0, 1, PI/3);
}

TEST_F(TransformTest, can_translate_and_rotate_a_point)
{
    kinematics::rot(0, 0, 1, PI/3);
}

TEST_F(TransformTest, can_only_use_a_transformation_on_a_screw_if_the_frames_match)
{

}

TEST_F(TransformTest, can_translate_a_screw)
{

}

TEST_F(TransformTest, can_rotate_a_screw)
{

}

TEST_F(TransformTest, can_translate_and_rotate_a_screw)
{

}

TEST_F(TransformTest, can_only_use_a_transformation_on_a_wrench_if_the_frames_match)
{

}

TEST_F(TransformTest, can_translate_a_wrench)
{

}

TEST_F(TransformTest, can_rotate_a_wrench)
{

}

TEST_F(TransformTest, can_translate_and_rotate_a_wrench)
{

}
