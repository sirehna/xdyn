/*
 * environment_parsersTest.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENT_PARSERSTEST_HPP_
#define ENVIRONMENT_PARSERSTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class environment_parsersTest : public ::testing::Test
{
    protected:
        environment_parsersTest();
	    virtual ~environment_parsersTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* ENVIRONMENT_PARSERSTEST_HPP_ */
