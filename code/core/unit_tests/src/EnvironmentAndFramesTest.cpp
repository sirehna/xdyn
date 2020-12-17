/*
 * EnvironmentAndFramesTest.cpp
 *
 *  Created on: 17 déc. 2020
 *      Author: mcharlou2016
 */

#include "EnvironmentAndFramesTest.hpp"

#include "EnvironmentAndFrames.hpp"
#include "InvalidInputException.hpp"

EnvironmentAndFramesTest::EnvironmentAndFramesTest ()
{}

EnvironmentAndFramesTest::~EnvironmentAndFramesTest ()
{}

TEST_F(EnvironmentAndFramesTest, can_get_rho_air_when_initialized)
{
	EnvironmentAndFrames env;
	env.set_rho_air(1.225);
	ASSERT_DOUBLE_EQ(1.225,env.get_rho_air());
}

TEST_F(EnvironmentAndFramesTest, throws_if_trying_to_get_uninitialized_rho_air)
{
	EnvironmentAndFrames env;
	ASSERT_THROW(env.get_rho_air(),InvalidInputException);
}
