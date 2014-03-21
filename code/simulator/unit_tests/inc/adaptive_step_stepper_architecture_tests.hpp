#ifndef ADAPTIVE_STEP_STEPPER_ARCHITECTURE_TESTS_HPP_
#define ADAPTIVE_STEP_STEPPER_ARCHITECTURE_TESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class adaptive_step_stepper_architecture_tests : public ::testing::Test
{
    protected:
        adaptive_step_stepper_architecture_tests();
        virtual ~adaptive_step_stepper_architecture_tests();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* ADAPTIVE_STEP_STEPPER_ARCHITECTURE_TESTS_HPP_ */
