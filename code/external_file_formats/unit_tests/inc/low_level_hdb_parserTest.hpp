/*
 * HDBParserTest.hpp
 *
 *  Created on: Nov 5, 2014
 *      Author: cady
 */

#ifndef low_level_hdb_parserTest_HPP_
#define low_level_hdb_parserTest_HPP_

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

#endif  /* low_level_hdb_parserTest_HPP_ */
