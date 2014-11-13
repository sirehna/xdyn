/*
 * HDBDataTest.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBDATATEST_HPP_
#define HDBDATATEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class HDBDataTest : public ::testing::Test
{
    protected:
        HDBDataTest();
        virtual ~HDBDataTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HDBDATATEST_HPP_ */
