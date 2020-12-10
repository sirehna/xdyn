/*
 * WindMeanVelocityProfileTest.hpp
 *
 *  Created on: 7 déc. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_UNIT_TESTS_UNIFORMWINDVELOCITYPROFILETEST_HPP_
#define ENVIRONMENT_MODELS_UNIT_TESTS_UNIFORMWINDVELOCITYPROFILETEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class WindMeanVelocityProfileTest : public ::testing::Test
{
public:
	WindMeanVelocityProfileTest ();
	virtual ~WindMeanVelocityProfileTest ();
	virtual void SetUp();
	virtual void TearDown();
	ssc::random_data_generator::DataGenerator a;
};

#endif /* ENVIRONMENT_MODELS_UNIT_TESTS_UNIFORMWINDVELOCITYPROFILETEST_HPP_ */
