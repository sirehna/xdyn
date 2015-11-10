

#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_SIMSERVERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_SIMSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class SimServerTest : public ::testing::Test
{
    protected:
        SimServerTest();
        virtual ~SimServerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* OBSERVERS_AND_API_UNIT_TESTS_INC_SIMSERVERTEST_HPP_ */
