#ifndef VELOCITYTEST_HPP_
#define VELOCITYTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class VelocityTest : public ::testing::Test
{
    protected:
        VelocityTest();
        virtual ~VelocityTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* VELOCITYTEST_HPP_ */
