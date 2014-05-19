/*
 * pairwise_sumTest.hpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#ifndef PAIRWISE_SUMTEST_HPP_
#define PAIRWISE_SUMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class pairwise_sumTest : public ::testing::Test
{
    protected:
        pairwise_sumTest();
	    virtual ~pairwise_sumTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* PAIRWISE_SUMTEST_HPP_ */
