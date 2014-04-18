#ifndef CONSTANT_STEP_STEPPER_ARCHITECTURE_TESTS_HPP_
#define CONSTANT_STEP_STEPPER_ARCHITECTURE_TESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class constant_step_stepper_architecture_tests : public ::testing::Test
{
    protected:
        constant_step_stepper_architecture_tests();
        virtual ~constant_step_stepper_architecture_tests();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* CONSTANT_STEP_STEPPER_ARCHITECTURE_TESTS_HPP_ */
