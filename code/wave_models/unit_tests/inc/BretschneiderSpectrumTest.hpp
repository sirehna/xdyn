/*
 * BretschneiderSpectrumTest.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef BRETSCHNEIDERSPECTRUMTEST_HPP_
#define BRETSCHNEIDERSPECTRUMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class BretschneiderSpectrumTest : public ::testing::Test
{
    protected:
        BretschneiderSpectrumTest();
        virtual ~BretschneiderSpectrumTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* BRETSCHNEIDERSPECTRUMTEST_HPP_ */
