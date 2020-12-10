/*
 * WindMeanVelocityProfileTest.cpp
 *
 *  Created on: 7 déc. 2020
 *      Author: mcharlou2016
 */

#include <string>
#include <math.h>

#include "UniformWindVelocityProfile.hpp"
#include "PowerLawWindVelocityProfile.hpp"

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
	ASSERT_DOUBLE_EQ(input.velocity*cos(input.direction),wind_vector(0));
	ASSERT_DOUBLE_EQ(input.velocity*sin(input.direction),wind_vector(1));
	ASSERT_DOUBLE_EQ(0.,wind_vector(2));
}

TEST_F(WindMeanVelocityProfileTest, can_parse_PowerLawWindVelocityProfile_input)
{
	std::string yaml_input = "{velocity: {unit: m/s, value: 8}, direction: {unit: rad, value: 1}, alpha: 0.11, reference height: {unit: m, value: 10}}";
	auto input = PowerLawWindVelocityProfile::parse(yaml_input);
	ASSERT_DOUBLE_EQ(input.velocity,8);
	ASSERT_DOUBLE_EQ(input.direction,1);
	ASSERT_DOUBLE_EQ(input.z_ref,10);
	ASSERT_DOUBLE_EQ(input.alpha,0.11);
}

TEST_F(WindMeanVelocityProfileTest, PowerLawWindVelocityProfile_returns_right_value)
{
	PowerLawWindVelocityProfile::Input input;
	input.velocity=8;
	input.direction=135*M_PI/180;
	input.alpha=0.11;
	input.z_ref=10;
	PowerLawWindVelocityProfile wind_model(input);
	Eigen::Vector3d position;
	position << a.random<double>(), a.random<double>(), -abs(a.random<double>());
	double time(a.random<double>());
	Eigen::Vector3d wind_vector(wind_model.get_wind(position,time));
	double wind_velocity = input.velocity*pow(-position(2)/input.z_ref,input.alpha);
	ASSERT_DOUBLE_EQ(wind_velocity*cos(input.direction),wind_vector(0));
	ASSERT_DOUBLE_EQ(wind_velocity*sin(input.direction),wind_vector(1));
	ASSERT_DOUBLE_EQ(0.,wind_vector(2));
}
