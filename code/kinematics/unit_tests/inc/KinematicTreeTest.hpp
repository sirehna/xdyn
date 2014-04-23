#ifndef KINEMATICTREETEST_HPP_
#define KINEMATICTREETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class KinematicTreeTest : public ::testing::Test
{
    protected:
        KinematicTreeTest();
	    virtual ~KinematicTreeTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* KINEMATICTREETEST_HPP_ */
