/*
 * StretchingTest.hpp
 *
 *  Created on: Dec 20, 2016
 *      Author: cady
 */

#ifndef STRETCHINGTEST_HPP_
#define STRETCHINGTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class StretchingTest : public ::testing::Test
{
    protected:
        StretchingTest();
        virtual ~StretchingTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* STRETCHINGTEST_HPP_ */
