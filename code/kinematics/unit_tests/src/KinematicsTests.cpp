/*
 * KinematicsTests.cpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#include "KinematicsTests.hpp"
#include "test_macros.hpp"
#include "kinematics.hpp"

KinematicsTests::KinematicsTests() : a(DataGenerator(122))
{
}

KinematicsTests::~KinematicsTests()
{
}

void KinematicsTests::SetUp()
{
}

void KinematicsTests::TearDown()
{
}

TEST_F(KinematicsTests, example)
{
//! [KinematicsTests example]
    const auto base = kinematics::FixedFrame::get_instance();
    const kinematics::Vector3DInFrame P(base, 1, 2, 3);
    rw::math::Rotation3D<double> R;

    const auto F1 = kinematics::Frame::make_new(P, R);
    kinematics::Velocity v(P,F1,base);

//! [KinematicsTests example]
//! [KinematicsTests expected output]
//! [KinematicsTests expected output]
}

TEST_F(KinematicsTests, DISABLED_can_get_coordinates_of_a_point_in_another_reference_frame_simply_translated)
{
    //! [KinematicsTests operator/]
    const auto base = kinematics::FixedFrame::get_instance();
    const kinematics::Vector3DInFrame u1(base, a.random<double>(),
                                                       a.random<double>(),
                                                       a.random<double>());
    const kinematics::Vector3DInFrame u2(base, a.random<double>(),
                                                       a.random<double>(),
                                                       a.random<double>());
    const auto F1 = kinematics::Frame::make_new(u1);
    const auto F2 = kinematics::Frame::make_new(u2);
    const kinematics::Vector3DInFrame P(F1, a.random<double>(),
                                                    a.random<double>(),
                                                    a.random<double>());

    const auto P_projected_in_F2 = P / F2;
    //! [KinematicsTests operator/]

    ASSERT_DOUBLE_EQ(u1[0]-u2[0]+P[0], P_projected_in_F2[0]);
    ASSERT_DOUBLE_EQ(u1[1]-u2[1]+P[1], P_projected_in_F2[1]);
    ASSERT_DOUBLE_EQ(u1[2]-u2[2]+P[2], P_projected_in_F2[2]);
}

TEST_F(KinematicsTests, DISABLED_can_get_coordinates_of_a_point_in_another_reference_frame_simply_rotated)
{
    ASSERT_TRUE(false);
}

TEST_F(KinematicsTests, DISABLED_can_get_coordinates_of_a_point_in_another_reference_frame_translated_and_rotated)
{
    ASSERT_TRUE(false);
}

TEST_F(KinematicsTests, DISABLED_should_be_able_to_perform_round_trip_transform_on_point_by_applying_inverse_transform)
{
    ASSERT_TRUE(false);
}
