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

TEST_F(force_parsersTest, gravity)
{
    const GravityParameters p = parse_gravity("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity");
    ASSERT_DOUBLE_EQ(9.81, p.g);
}

TEST_F(force_parsersTest, hydrostatic)
{
    const double rho = parse_hydrostatic("rho:\n  unit: kg/m^3\n  value: 1024\nmodel: hydrostatic");
    ASSERT_DOUBLE_EQ(1024, rho);
}

