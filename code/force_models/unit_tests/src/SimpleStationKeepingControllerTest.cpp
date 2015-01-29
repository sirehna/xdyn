/*
 * SimpleStationKeepingControllerTest.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: cady
 */


#include "SimpleStationKeepingControllerTest.hpp"
#include "SimpleStationKeepingController.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define EPS 1E-14

SimpleStationKeepingControllerTest::SimpleStationKeepingControllerTest() : a(ssc::random_data_generator::DataGenerator(85552))
{
}

SimpleStationKeepingControllerTest::~SimpleStationKeepingControllerTest()
{
}

void SimpleStationKeepingControllerTest::SetUp()
{
}

void SimpleStationKeepingControllerTest::TearDown()
{
}

TEST_F(SimpleStationKeepingControllerTest, parser)
{
    const auto k = SimpleStationKeepingController::parse(test_data::simple_station_keeping());
    ASSERT_DOUBLE_EQ(2, k.T_x);
    ASSERT_DOUBLE_EQ(3, k.T_y);
    ASSERT_DOUBLE_EQ(4, k.T_psi);
    ASSERT_DOUBLE_EQ(0.9, k.ksi_x);
    ASSERT_DOUBLE_EQ(0.85, k.ksi_y);
    ASSERT_DOUBLE_EQ(0.8, k.ksi_psi);
    ASSERT_EQ("controller", k.name);
}

TEST_F(SimpleStationKeepingControllerTest, force_and_torque)
{
    auto input = SimpleStationKeepingController::parse(test_data::simple_station_keeping());
    input.T_x = 2*PI;
    input.T_y = 2*PI/3;
    input.T_psi = 2*PI/4;
    EnvironmentAndFrames env;
    const SimpleStationKeepingController w(input, "body", env);
    BodyStates states;
    const double x = 0.456;
    const double y = 5.769;
    const double psi = 1.234;
    states.qr.record(0,  cos(psi/2));
    states.qi.record(0,  0);
    states.qj.record(0,  0);
    states.qk.record(0,  sin(psi/2));
    states.u.record(0, 8);
    states.v.record(0, 9);
    states.r.record(0, 10);
    states.x.record(0, x);
    states.y.record(0, y);
    states.total_inertia = MatrixPtr(new Eigen::Matrix<double,6,6>());
    states.total_inertia->operator()(0,0) = 4;
    states.total_inertia->operator()(1,1) = 5;
    states.total_inertia->operator()(5,5) = 6;

    std::map<std::string,double> commands;
    commands["x_co"] = 5;
    commands["y_co"] = 6;
    commands["psi_co"] = 7;

    const auto F = w.get_force(states, a.random<double>(),commands);

    ASSERT_NEAR(4*1*(5-x)-2*0.9*4*1*8, (double)F(0), EPS);
    ASSERT_NEAR(5*9*(6-y)-2*0.85*5*3*9, (double)F(1), EPS);
    ASSERT_NEAR(0, (double)F(2), EPS);
    ASSERT_NEAR(0, (double)F(3), EPS);
    ASSERT_NEAR(0, (double)F(4), EPS);
    ASSERT_NEAR(6*16*(7-psi)-2*0.8*6*4*10, (double)F(5), EPS);
}



