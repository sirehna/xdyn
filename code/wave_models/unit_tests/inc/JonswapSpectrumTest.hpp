/*
 * JonswapSpectrumTest.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef JONSWAPSPECTRUMTEST_HPP_
#define JONSWAPSPECTRUMTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class JonswapSpectrumTest : public ::testing::Test
{
    protected:
        JonswapSpectrumTest();
        virtual ~JonswapSpectrumTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* JONSWAPSPECTRUMTEST_HPP_ */
