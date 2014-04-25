/*
 * WrenchTest.hpp
 *
 *  Created on: Apr 24, 2014
 *      Author: cady
 */

#ifndef WRENCHTEST_HPP_
#define WRENCHTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class WrenchTest : public ::testing::Test
{
    protected:
        WrenchTest();
	    virtual ~WrenchTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* WRENCHTEST_HPP_ */
