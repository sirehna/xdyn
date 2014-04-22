#ifndef DEFAULTWAVEMODELTEST_HPP_
#define DEFAULTWAVEMODELTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DefaultWaveModelTest : public ::testing::Test
{
    protected:
        DefaultWaveModelTest();
	    virtual ~DefaultWaveModelTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* DEFAULTWAVEMODELTEST_HPP_ */
