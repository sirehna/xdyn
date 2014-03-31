#ifndef HYDROSTATICFORCETEST_HPP_
#define HYDROSTATICFORCETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class HydrostaticForceTest : public ::testing::Test
{
    protected:
        HydrostaticForceTest();
        virtual ~HydrostaticForceTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* HYDROSTATICFORCETEST_HPP_ */
