/*
 * force_parsersTest.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "force_parsersTest.hpp"
#include "force_parsers.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define DEG2RAD (PI/180.)

force_parsersTest::force_parsersTest() : a(ssc::random_data_generator::DataGenerator(86556))
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

TEST_F(force_parsersTest, wageningen)
{
    const YamlWageningen w = parse_wageningen(test_data::wageningen());
    ASSERT_DOUBLE_EQ(0.5, w.blade_area_ratio);
    ASSERT_EQ("port side propeller", w.name);
    ASSERT_EQ(3, w.number_of_blades);
    ASSERT_DOUBLE_EQ(0, w.position_of_propeller_frame.angle.phi);
    ASSERT_DOUBLE_EQ(-10*DEG2RAD, w.position_of_propeller_frame.angle.theta);
    ASSERT_DOUBLE_EQ(-1*DEG2RAD, w.position_of_propeller_frame.angle.psi);
    ASSERT_DOUBLE_EQ(-4, w.position_of_propeller_frame.coordinates.x);
    ASSERT_DOUBLE_EQ(-2, w.position_of_propeller_frame.coordinates.y);
    ASSERT_DOUBLE_EQ(2, w.position_of_propeller_frame.coordinates.z);
    ASSERT_EQ("mesh(body 1)", w.position_of_propeller_frame.frame);
    ASSERT_DOUBLE_EQ(1, w.relative_rotative_efficiency);
    ASSERT_TRUE(w.rotating_clockwise);
    ASSERT_DOUBLE_EQ(0.7, w.thrust_deduction_factor);
    ASSERT_DOUBLE_EQ(0.9, w.wake_coefficient);
    ASSERT_DOUBLE_EQ(2, w.diameter);
}
