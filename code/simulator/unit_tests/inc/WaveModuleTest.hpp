/*
 * GravityModuleTest.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef WAVEMODULETEST_HPP_
#define WAVEMODULETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class WaveModuleTest : public ::testing::Test
{
    protected:
        WaveModuleTest();
	    virtual ~WaveModuleTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* WAVEMODULETEST_HPP_ */
