/*
 * coriolis_and_centripetalTest.hpp
 *
 *  Created on: Jun 3, 2014
 *      Author: cady
 */

#ifndef CORIOLIS_AND_CENTRIPETALTEST_HPP_
#define CORIOLIS_AND_CENTRIPETALTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class coriolis_and_centripetalTest : public ::testing::Test
{
    protected:
        coriolis_and_centripetalTest();
	    virtual ~coriolis_and_centripetalTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* CORIOLIS_AND_CENTRIPETALTEST_HPP_ */
