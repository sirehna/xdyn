/*
 * KinematicsTests.cpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#include "KinematicsTests.hpp"
#include "Kinematics.hpp"
#include "random_kinematics.hpp"

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

TEST_F(KinematicsTests, can_add_a_transform_to_a_kinematics_object)
{
    Kinematics k;
    const auto bTa = random_transform(a, a.random<std::string>(), a.random<std::string>());
    k.add(bTa);
}
