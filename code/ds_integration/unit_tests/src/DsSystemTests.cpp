/*
 * DsSystemTests.cpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#include "DsSystemTests.hpp"
#include "DsSystemMacros.hpp"
#include "DataSource.hpp"
#include "DataSourceModule.hpp"

#include "test_macros.hpp"


DsSystemTests::DsSystemTests() : a(DataGenerator(87514651))
{
}

DsSystemTests::~DsSystemTests()
{
}

void DsSystemTests::SetUp()
{
}

void DsSystemTests::TearDown()
{
}

DEFINE(falling_ball, z, double)
DEFINE(falling_ball, zdot, double)
DEFINE(falling_ball, zdotdot, double)

TEST_F(DsSystemTests, example)
{
//! [DsSystemTests example]
    DataSource ds;
    SET(ds, falling_ball::zdotdot, -9.81);
    SET(ds, falling_ball::zdot, 0);
    SET(ds, falling_ball::z, 0);
//! [DsSystemTests example]
//! [DsSystemTests expected output]
//! [DsSystemTests expected output]
}
