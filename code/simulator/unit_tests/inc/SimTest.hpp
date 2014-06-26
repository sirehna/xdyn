/*
 * SimTest.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMTEST_HPP_
#define SIMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class SimTest : public ::testing::Test
{
    protected:
        SimTest();
	    virtual ~SimTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* SIMTEST_HPP_ */
