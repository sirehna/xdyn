/*
 * force_parsersTest.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "force_parsersTest.hpp"
#include "force_parsers.hpp"

force_parsersTest::force_parsersTest() : a(DataGenerator(86556))
{
}

force_parsersTest::~force_parsersTest()
{
}

void force_parsersTest::SetUp()
{
}

void force_parsersTest::TearDown()
{
}

TEST_F(force_parsersTest, example)
{
//! [force_parsersTest example]
    const GravityParameters p = parse_gravity("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity");
//! [force_parsersTest example]
//! [force_parsersTest expected output]
    ASSERT_DOUBLE_EQ(9.81, p.g);
//! [force_parsersTest expected output]
}



