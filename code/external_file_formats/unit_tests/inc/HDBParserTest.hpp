/*
 * HDBParserTest.hpp
 *
 *  Created on: Nov 5, 2014
 *      Author: cady
 */

#ifndef HDBPARSERTEST_HPP_
#define HDBPARSERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class low_level_hdb_parserTest : public ::testing::Test
{
    protected:
        low_level_hdb_parserTest();
        virtual ~low_level_hdb_parserTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HDBPARSERTEST_HPP_ */
