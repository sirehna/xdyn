/*
 * SimulatorBuilderTest.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDERTEST_HPP_
#define SIMULATORBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class SimulatorBuilderTest : public ::testing::Test
{
    protected:
        SimulatorBuilderTest();
	    virtual ~SimulatorBuilderTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* SIMULATORBUILDERTEST_HPP_ */
