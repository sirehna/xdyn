#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_CONFBUILDERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_CONFBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class ConfBuilderTest : public ::testing::Test
{
    protected:
        ConfBuilderTest();
        virtual ~ConfBuilderTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* OBSERVERS_AND_API_UNIT_TESTS_INC_CONFBUILDERTEST_HPP_ */
