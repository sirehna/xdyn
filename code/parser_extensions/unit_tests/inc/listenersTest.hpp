/*
 * listenersTest.hpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#ifndef LISTENERSTEST_HPP_
#define LISTENERSTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class listenersTest : public ::testing::Test
{
    protected:
        listenersTest();
        virtual ~listenersTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* LISTENERSTEST_HPP_ */
