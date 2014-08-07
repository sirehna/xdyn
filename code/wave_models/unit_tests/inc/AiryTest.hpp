/*
 * AiryTest.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef AIRYTEST_HPP_
#define AIRYTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class AiryTest : public ::testing::Test
{
    protected:
        AiryTest();
        virtual ~AiryTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* AIRYTEST_HPP_ */
