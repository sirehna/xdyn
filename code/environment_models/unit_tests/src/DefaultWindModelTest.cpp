/*
 * DefaultWindModelTest.cpp
 *
 *  Created on: 23 nov. 2020
 *      Author: mcharlou2016
 */

#include <Eigen/Dense>

#include "DefaultWindModel.hpp"

#include "DefaultWindModelTest.hpp"

DefaultWindModelTest::DefaultWindModelTest () : a(ssc::random_data_generator::DataGenerator(54200))
{}

DefaultWindModelTest::~DefaultWindModelTest (){}

void DefaultWindModelTest::SetUp(){}

void DefaultWindModelTest::TearDown(){}

TEST_F(DefaultWindModelTest, returns_no_wind)
{
	DefaultWindModel wind_model(0);
	Eigen::Vector3d position;
	position << a.random<double>(), a.random<double>(), a.random<double>();
	double time(a.random<double>());
	Eigen::Vector3d wind_vector(wind_model.get_wind(position,time));
	ASSERT_DOUBLE_EQ(0.,wind_vector(0));
	ASSERT_DOUBLE_EQ(0.,wind_vector(1));
	ASSERT_DOUBLE_EQ(0.,wind_vector(2));
}
