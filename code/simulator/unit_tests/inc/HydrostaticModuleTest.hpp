/*
 * GravityModuleTest.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICMODULETEST_HPP_
#define HYDROSTATICMODULETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class HydrostaticModuleTest : public ::testing::Test
{
    protected:
        HydrostaticModuleTest();
	    virtual ~HydrostaticModuleTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* HYDROSTATICMODULETEST_HPP_ */
