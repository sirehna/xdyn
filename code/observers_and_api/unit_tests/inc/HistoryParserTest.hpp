/*
 * HistoryParserTest.hpp
 *
 *  Created on: 28 oct. 2015
 *      Author: sirehna
 */


#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_HISTORYPARSERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_HISTORYPARSERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class HistoryParserTest : public ::testing::Test
{
    protected:
        HistoryParserTest();
        virtual ~HistoryParserTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* OBSERVERS_AND_API_UNIT_TESTS_INC_HISTORYPARSERTEST_HPP_ */
