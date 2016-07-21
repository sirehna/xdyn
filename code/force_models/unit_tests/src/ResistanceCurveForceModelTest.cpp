/*
 * ResistanceCurveForceModelTest.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "ResistanceCurveForceModel.hpp"
#include "ResistanceCurveForceModelTest.hpp"
#include "yaml_data.hpp"
#include "EnvironmentAndFrames.hpp"

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

TEST_F(ResistanceCurveForceModelTest, parser)
{
    const auto r = ResistanceCurveForceModel::parse(test_data::resistance_curve());
    ASSERT_EQ(8, r.R.size());
    ASSERT_EQ(8, r.Va.size());

    const double knot = 0.51444444444444444;
    ASSERT_DOUBLE_EQ(0, r.Va[0]);
    ASSERT_DOUBLE_EQ(1*knot, r.Va[1]);
    ASSERT_DOUBLE_EQ(2*knot, r.Va[2]);
    ASSERT_DOUBLE_EQ(3*knot, r.Va[3]);
    ASSERT_DOUBLE_EQ(4*knot, r.Va[4]);
    ASSERT_DOUBLE_EQ(5*knot, r.Va[5]);
    ASSERT_DOUBLE_EQ(15*knot, r.Va[6]);
    ASSERT_DOUBLE_EQ(20*knot, r.Va[7]);

    ASSERT_DOUBLE_EQ(0, r.R[0]);
    ASSERT_DOUBLE_EQ(1E6, r.R[1]);
    ASSERT_DOUBLE_EQ(4E6, r.R[2]);
    ASSERT_DOUBLE_EQ(9E6, r.R[3]);
    ASSERT_DOUBLE_EQ(16E6, r.R[4]);
    ASSERT_DOUBLE_EQ(25E6, r.R[5]);
    ASSERT_DOUBLE_EQ(225E6, r.R[6]);
    ASSERT_DOUBLE_EQ(400E6, r.R[7]);
}

TEST_F(ResistanceCurveForceModelTest, example)
{
//! [ResistanceCurveForceModelTest example]
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", EnvironmentAndFrames());
//! [ResistanceCurveForceModelTest example]
    ASSERT_EQ("resistance curve",F.model_name());
//! [ResistanceCurveForceModelTest expected output]
    BodyStates states;
    states.u.record(0, 0);
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).X());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).Y());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).Z());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).K());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).M());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).N());

    states.u.record(1, 0.514444444444444);
    ASSERT_NEAR(-1e6, (double)F(states, a.random<double>()).X(), EPS);
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).Y());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).Z());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).K());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).M());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).N());


    states.u.record(2, 0.514444444444444*15);
    ASSERT_NEAR(-225e6, (double)F(states, a.random<double>()).X(), EPS);
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).Y());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).Z());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).K());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).M());
    ASSERT_DOUBLE_EQ(0, (double)F(states, a.random<double>()).N());
//! [ResistanceCurveForceModelTest expected output]
}

TEST_F(ResistanceCurveForceModelTest, should_issue_a_warning_when_speed_is_lower_than_min_speed_specified_in_resistance_curve_table)
{
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", EnvironmentAndFrames());
    BodyStates states;
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    ASSERT_TRUE(error.str().empty());
    // Call the resistance curve model
    states.u.record(0, -1);
    F(states, a.random<double>());
    ASSERT_FALSE(error.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(ResistanceCurveForceModelTest, should_issue_a_warning_when_speed_is_greater_than_max_speed_specified_in_resistance_curve_table)
{
    ResistanceCurveForceModel F(ResistanceCurveForceModel::parse(test_data::resistance_curve()), "", EnvironmentAndFrames());
    BodyStates states;
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    ASSERT_TRUE(error.str().empty());
    // Call the resistance curve model
    states.u.record(0, 20*0.514444444444444444 + 1E-10);
    F(states, a.random<double>());
    ASSERT_FALSE(error.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}
