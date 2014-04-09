/*
 * VelocityTest.cpp
 *
 *  Created on: 9 avr. 2014
 *      Author: maroff
 */

#include "VelocityTest.hpp"
#include "Velocity.hpp"
#include "extra_test_assertions.hpp"
#include "KinematicsException.hpp"

#define EPS 1E-13

VelocityTest::VelocityTest() : a(DataGenerator(7779))
{
}

VelocityTest::~VelocityTest()
{
}

void VelocityTest::SetUp()
{
}

void VelocityTest::TearDown()
{
}

TEST_F(VelocityTest, throws_if_P_and_omega_are_not_projected_in_the_same_frame)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const std::string projection_frame = a.random<std::string>();
        const Point P(projection_frame,1,2,3);
        const AngularVelocityVector omega_in_same_frame(projection_frame, a.random<double>()(),a.random<double>()(),a.random<double>()());
        const AngularVelocityVector omega_in_different_frame(a.random<std::string>(), a.random<double>()(),a.random<double>()(),a.random<double>()());
        ASSERT_NO_THROW(Velocity(P, omega_in_same_frame));
        ASSERT_THROW(Velocity(P, omega_in_different_frame), KinematicsException);
    }
}
