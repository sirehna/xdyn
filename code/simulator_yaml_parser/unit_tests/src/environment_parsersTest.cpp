/*
 * environment_parsersTest.cpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#include "environment_parsersTest.hpp"
#include "environment_parsers.hpp"

environment_parsersTest::environment_parsersTest() : a(DataGenerator(1212))
{
}

environment_parsersTest::~environment_parsersTest()
{
}

void environment_parsersTest::SetUp()
{
}

void environment_parsersTest::TearDown()
{
}

TEST_F(environment_parsersTest, example)
{
//! [environment_parsersTest example]
    const double zwave = parse_default_wave_model("constant wave height in NED frame: {value: 1234, unit: m}");
//! [environment_parsersTest example]
//! [environment_parsersTest expected output]
    ASSERT_DOUBLE_EQ(1234,zwave);
//! [environment_parsersTest expected output]
}



