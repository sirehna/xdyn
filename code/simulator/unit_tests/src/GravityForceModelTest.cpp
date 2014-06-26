/*
 * GravityForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "GravityForceModel.hpp"
#include "GravityForceModelTest.hpp"
#include "generate_body_for_tests.hpp"

#define BODY "body 1"

GravityForceModelTest::GravityForceModelTest() : a(DataGenerator(45454))
{
}

GravityForceModelTest::~GravityForceModelTest()
{
}

void GravityForceModelTest::SetUp()
{
}

void GravityForceModelTest::TearDown()
{
}

TEST_F(GravityForceModelTest, example)
{
//! [GravityForceModelTest example]
    GravityForceModel F(9.81);
    Body b = get_body(BODY);
    b.m = 100;
    const Wrench f = F(b);
//! [GravityForceModelTest example]
//! [GravityForceModelTest expected output]
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_DOUBLE_EQ(0, f.K());
    ASSERT_DOUBLE_EQ(0, f.M());
    ASSERT_DOUBLE_EQ(0, f.N());
    ASSERT_DOUBLE_EQ(0, f.X());
    ASSERT_DOUBLE_EQ(0, f.Y());
    ASSERT_DOUBLE_EQ(981, f.Z());
//! [GravityForceModelTest expected output]
}

