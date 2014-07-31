/*
 * WaveNumberFunctorTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef WAVENUMBERFUNCTORTEST_HPP_
#define WAVENUMBERFUNCTORTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class WaveNumberFunctorTest : public ::testing::Test
{
    protected:
        WaveNumberFunctorTest();
        virtual ~WaveNumberFunctorTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* WAVENUMBERFUNCTORTEST_HPP_ */
