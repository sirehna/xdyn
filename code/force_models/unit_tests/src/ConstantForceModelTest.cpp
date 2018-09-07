/*
 * GravityForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "ConstantForceModel.hpp"
#include "ConstantForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include <ssc/kinematics.hpp>

#define BODY "body 1"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

ConstantForceModelTest::ConstantForceModelTest() : a(ssc::random_data_generator::DataGenerator(45454))
{
}

ConstantForceModelTest::~ConstantForceModelTest()
{
}

void ConstantForceModelTest::SetUp()
{
}

void ConstantForceModelTest::TearDown()
{
}

TEST_F(ConstantForceModelTest, example)
{
}
