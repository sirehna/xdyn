/*
 * hydrostaticTest.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICTEST_HPP_
#define HYDROSTATICTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class hydrostaticTest : public ::testing::Test
{
    protected:
        hydrostaticTest();
	    virtual ~hydrostaticTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* HYDROSTATICTEST_HPP_ */
