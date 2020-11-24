/*
 * DefaultWindModelTest.hpp
 *
 *  Created on: 23 nov. 2020
 *      Author: mcharlou2016
 */

#ifndef ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_
#define ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class DefaultWindModelTest : public ::testing::Test
{
protected:
	DefaultWindModelTest ();
	virtual ~DefaultWindModelTest ();
	virtual void SetUp();
	virtual void TearDown();
	ssc::random_data_generator::DataGenerator a;
};

#endif /* ENVIRONMENT_MODELS_UNIT_TESTS_INC_DEFAULTWINDMODELTEST_HPP_ */
