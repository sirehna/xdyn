/*
 * HDBParserTest.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBPARSERTEST_HPP_
#define HDBPARSERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class HDBParserTest : public ::testing::Test
{
    protected:
        HDBParserTest();
        virtual ~HDBParserTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HDBPARSERTEST_HPP_ */
