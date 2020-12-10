/*
 * WindMeanVelocityProfileTest.cpp
 *
 *  Created on: 7 déc. 2020
 *      Author: mcharlou2016
 */

#include <string>
#include <math.h>

#include "UniformWindVelocityProfile.hpp"

#include "WindMeanVelocityProfileTest.hpp"

WindMeanVelocityProfileTest::WindMeanVelocityProfileTest() : a(ssc::random_data_generator::DataGenerator(54200))
{}

WindMeanVelocityProfileTest::~WindMeanVelocityProfileTest()
{}

void WindMeanVelocityProfileTest::SetUp(){}

void WindMeanVelocityProfileTest::TearDown(){}

TEST_F(WindMeanVelocityProfileTest, can_parse_UniformWindVelocityProfile_input)
{
	std::string yaml_input = "{velocity: {unit: m/s, value: 10}, direction: {unit: rad, value: 1}}";
	auto input = UniformWindVelocityProfile::parse(yaml_input);
	ASSERT_DOUBLE_EQ(input.velocity,10);
	ASSERT_DOUBLE_EQ(input.direction,1);
}

TEST_F(WindMeanVelocityProfileTest, UniformWindVelocityProfile_returns_right_value)
{
	UniformWindVelocityProfile::Input input;
	input.velocity=10;
	input.direction=135*M_PI/180;
	UniformWindVelocityProfile wind_model(input);
	Eigen::Vector3d position;
	position << a.random<double>(), a.random<double>(), a.random<double>();
	double time(a.random<double>());
	Eigen::Vector3d wind_vector(wind_model.get_wind(position,time));
	ASSERT_DOUBLE_EQ(10*cos(input.direction),wind_vector(0));
	ASSERT_DOUBLE_EQ(10*sin(input.direction),wind_vector(1));
	ASSERT_DOUBLE_EQ(0.,wind_vector(2));
}
