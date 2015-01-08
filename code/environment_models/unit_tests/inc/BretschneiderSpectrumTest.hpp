/*
 * BretschneiderSpectrumTest.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef BRETSCHNEIDERSPECTRUMTEST_HPP_
#define BRETSCHNEIDERSPECTRUMTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class BretschneiderSpectrumTest : public ::testing::Test
{
    protected:
        BretschneiderSpectrumTest();
        virtual ~BretschneiderSpectrumTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* BRETSCHNEIDERSPECTRUMTEST_HPP_ */
