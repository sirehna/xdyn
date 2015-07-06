/*
 * SimpleTrackKeepingControllerTest.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "BodyStates.hpp"
#include "SimpleHeadingKeepingControllerTest.hpp"
#include "SimpleHeadingKeepingController.hpp"
#include "yaml_data.hpp"

#define EPS 1E-14

SimpleHeadingKeepingControllerTest::SimpleHeadingKeepingControllerTest() : a(ssc::random_data_generator::DataGenerator(545454))
{
}

SimpleHeadingKeepingControllerTest::~SimpleHeadingKeepingControllerTest()
{
}

void SimpleHeadingKeepingControllerTest::SetUp()
{
}

void SimpleHeadingKeepingControllerTest::TearDown()
{
}

TEST_F(SimpleHeadingKeepingControllerTest, parser)
{
    const auto k = SimpleHeadingKeepingController::parse(test_data::simple_track_keeping());
    ASSERT_DOUBLE_EQ(4, k.Tp);
    ASSERT_DOUBLE_EQ(0.9, k.ksi);
    ASSERT_EQ("controller", k.name);
}

TEST_F(SimpleHeadingKeepingControllerTest, force_and_torque)
{
    auto input = SimpleHeadingKeepingController::parse(test_data::simple_track_keeping());
    input.Tp = 2*PI;
    EnvironmentAndFrames env;
    const SimpleHeadingKeepingController w(input, "body", env);
    ASSERT_EQ("simple heading controller", w.model_name);
    BodyStates states;
    const double psi = 1.234;
    states.qr.record(0,  cos(psi/2));
    states.qi.record(0,  0);
    states.qj.record(0,  0);
    states.qk.record(0,  sin(psi/2));
    states.r.record(0, 10);
    states.total_inertia = MatrixPtr(new Eigen::Matrix<double,6,6>());
    states.total_inertia->operator()(2,2) = 4;

    std::map<std::string,double> commands;
    commands["psi_co"] = 5;

    const auto F = w.get_force(states, a.random<double>(),commands);

    ASSERT_NEAR(0, (double)F(0), EPS);
    ASSERT_NEAR(0, (double)F(1), EPS);
    ASSERT_NEAR(0, (double)F(2), EPS);
    ASSERT_NEAR(0, (double)F(3), EPS);
    ASSERT_NEAR(0, (double)F(4), EPS);
    ASSERT_NEAR(4*1*(5-psi)-2*0.9*4*1*10, (double)F(5), EPS);
}
