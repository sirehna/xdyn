/*
 * GravityModuleTest.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#ifndef GRAVITYMODULETEST_HPP_
#define GRAVITYMODULETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class GravityModuleTest : public ::testing::Test
{
    protected:
        GravityModuleTest();
	    virtual ~GravityModuleTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* GRAVITYMODULETEST_HPP_ */
