/*
 * ResistanceCurveForceModelTest.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "force_parsers.hpp"
#include "ResistanceCurveForceModel.hpp"
#include "ResistanceCurveForceModelTest.hpp"
#include "yaml_data.hpp"
#include "YamlResistanceCurve.hpp"

#define EPS 1E-6

ResistanceCurveForceModelTest::ResistanceCurveForceModelTest() : a(ssc::random_data_generator::DataGenerator(89451))
{
}

ResistanceCurveForceModelTest::~ResistanceCurveForceModelTest()
{
}

void ResistanceCurveForceModelTest::SetUp()
{
}

void ResistanceCurveForceModelTest::TearDown()
{
}

TEST_F(ResistanceCurveForceModelTest, example)
{
//! [ResistanceCurveForceModelTest example]
    ResistanceCurveForceModel F(parse_resistance_curve(test_data::resistance_curve()));
//! [ResistanceCurveForceModelTest example]
//! [ResistanceCurveForceModelTest expected output]
    Body b;
    b.u = 0;
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).X());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).Y());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).Z());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).K());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).M());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).N());

    b.u = 0.514444444444444;
    ASSERT_NEAR(-1e6, (double)F(b, a.random<double>()).X(), EPS);
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).Y());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).Z());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).K());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).M());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).N());


    b.u = 0.514444444444444*15;
    ASSERT_NEAR(-225e6, (double)F(b, a.random<double>()).X(), EPS);
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).Y());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).Z());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).K());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).M());
    ASSERT_DOUBLE_EQ(0, (double)F(b, a.random<double>()).N());
//! [ResistanceCurveForceModelTest expected output]
}



