/*
 * HistoryTest.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */


#ifndef HISTORYTEST_HPP_
#define HISTORYTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class HistoryTest : public ::testing::Test
{
    protected:
        HistoryTest();
        virtual ~HistoryTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HISTORYTEST_HPP_ */
