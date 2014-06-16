/*
 * rotation_matrix_builders_Test.cpp
 *
 *  Created on: 7 avr. 2014
 *      Author: cec
 */

#include "rotation_matrix_builders_Test.hpp"
#include "rotation_matrix_builders.hpp"
#include "EulerAngles.hpp"
#include "test_macros.hpp"
#include <cmath>

rotation_matrix_builders_Test::rotation_matrix_builders_Test() : a(DataGenerator(545))
{
}

rotation_matrix_builders_Test::~rotation_matrix_builders_Test()
{
}

void rotation_matrix_builders_Test::SetUp()
{
}

void rotation_matrix_builders_Test::TearDown()
{
}

#define PI (4.*atan(1))

TEST_F(rotation_matrix_builders_Test, can_get_rotation_around_x_axis)
{
    using namespace kinematics;
    const RotationMatrix R = rot(1,0,0,PI/3);
    ASSERT_DOUBLE_EQ(1, R(0,0));
    ASSERT_DOUBLE_EQ(0, R(1,0));
    ASSERT_DOUBLE_EQ(0, R(2,0));
    ASSERT_DOUBLE_EQ(0, R(0,1));
    ASSERT_DOUBLE_EQ(0.5, R(1,1));
    ASSERT_DOUBLE_EQ(sqrt(3)/2, R(2,1));
    ASSERT_DOUBLE_EQ(0, R(0,2));
    ASSERT_DOUBLE_EQ(-sqrt(3)/2, R(1,2));
    ASSERT_DOUBLE_EQ(0.5, R(2,2));
}

TEST_F(rotation_matrix_builders_Test, can_get_rotation_around_y_axis)
{
    using namespace kinematics;
    const RotationMatrix R = rot(0,1,0,PI/3);
    ASSERT_DOUBLE_EQ(0.5, R(0,0));
    ASSERT_DOUBLE_EQ(0, R(1,0));
    ASSERT_DOUBLE_EQ(-sqrt(3)/2, R(2,0));
    ASSERT_DOUBLE_EQ(0, R(0,1));
    ASSERT_DOUBLE_EQ(1, R(1,1));
    ASSERT_DOUBLE_EQ(0, R(2,1));
    ASSERT_DOUBLE_EQ(sqrt(3)/2, R(0,2));
    ASSERT_DOUBLE_EQ(0, R(1,2));
    ASSERT_DOUBLE_EQ(0.5, R(2,2));
}

TEST_F(rotation_matrix_builders_Test, can_get_rotation_around_z_axis)
{
    using namespace kinematics;
    const RotationMatrix R = rot(0,0,1,PI/3);
    ASSERT_DOUBLE_EQ(0.5, R(0,0));
    ASSERT_DOUBLE_EQ(sqrt(3)/2, R(1,0));
    ASSERT_DOUBLE_EQ(0, R(2,0));
    ASSERT_DOUBLE_EQ(-sqrt(3)/2, R(0,1));
    ASSERT_DOUBLE_EQ(0.5, R(1,1));
    ASSERT_DOUBLE_EQ(0, R(2,1));
    ASSERT_DOUBLE_EQ(0, R(0,2));
    ASSERT_DOUBLE_EQ(0, R(1,2));
    ASSERT_DOUBLE_EQ(1, R(2,2));
}

TEST_F(rotation_matrix_builders_Test, example)
{
//! [rotation_matrix_builders_Test example]
    using namespace kinematics;
    const EulerAngles angles(PI/3, PI/4, PI/6);
    //const EulerAngles angles(PI/2,0,0);
    const RotationMatrix R = rotation_matrix<INTRINSIC, CHANGING_ANGLE_ORDER, CARDAN, 3, 2, 1>(angles);
//! [rotation_matrix_builders_Test example]
//! [rotation_matrix_builders_Test expected output]
    ASSERT_DOUBLE_EQ(sqrt(6)/4, R(0,0));
    ASSERT_DOUBLE_EQ(sqrt(3)/4+sqrt(6)/8, R(1,1));
    ASSERT_DOUBLE_EQ(sqrt(2)/4, R(2,2));
//! [rotation_matrix_builders_Test expected output]
}
