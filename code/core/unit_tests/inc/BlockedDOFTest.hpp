/*
 * BlockedDOFTest.hpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */


#ifndef BLOCKEDDOFTEST_HPP_
#define BLOCKEDDOFTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class BlockedDOFTest : public ::testing::Test
{
    protected:
        BlockedDOFTest();
        virtual ~BlockedDOFTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* BLOCKEDDOFTEST_HPP_ */
