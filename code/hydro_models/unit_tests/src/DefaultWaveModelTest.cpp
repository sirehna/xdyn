/*
 * HydrostaticForceTest.cpp
 *
 *  Created on: 31 mars 2014
 *      Author: maroff
 */

#include "DefaultWaveModelTest.hpp"
#include "DefaultWaveModel.hpp"
#include "Point.hpp"

DefaultWaveModelTest::DefaultWaveModelTest() : a(DataGenerator(5466123))
{
}

DefaultWaveModelTest::~DefaultWaveModelTest()
{
}

void DefaultWaveModelTest::SetUp()
{
}

void DefaultWaveModelTest::TearDown()
{
}

TEST_F(DefaultWaveModelTest, example)
{
//! [DefaultWaveModelTest example]
    const Point P("A", 0, 0, 0);
    const DefaultWaveModel w(0);
//! [DefaultWaveModelTest example]
//! [DefaultWaveModelTest expected output]
    ASSERT_DOUBLE_EQ(0, w.get_relative_wave_height(P));
//! [DefaultWaveModelTest expected output]
}
