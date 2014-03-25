#ifndef DSSYSTEMTESTS_HPP_
#define DSSYSTEMTESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DsSystemTests : public ::testing::Test
{
    protected:
        DsSystemTests();
        virtual ~DsSystemTests();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* DSSYSTEMTESTS_HPP_ */
