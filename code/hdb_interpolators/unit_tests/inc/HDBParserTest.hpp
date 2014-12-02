/*
 * HDBBuilderTest.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBBUILDERTEST_HPP_
#define HDBBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class HDBBuilderTest : public ::testing::Test
{
    protected:
        HDBBuilderTest();
        virtual ~HDBBuilderTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HDBBUILDERTEST_HPP_ */
