#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_SIMSTEPPERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_SIMSTEPPERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class SimStepperTest : public ::testing::Test
{
    protected:
        SimStepperTest();
        virtual ~SimStepperTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* OBSERVERS_AND_API_UNIT_TESTS_INC_SIMSTEPPERTEST_HPP_ */
