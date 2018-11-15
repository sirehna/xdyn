/*
 * XdynForMETest.hpp
 *
 *  Created on: Nov 15, 2018
 *      Author: cady
 */

#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_XDYNFORMETEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_XDYNFORMETEST_HPP_


#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class XdynForMETest : public ::testing::Test
{
    protected:
        XdynForMETest();
        virtual ~XdynForMETest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};


#endif /* OBSERVERS_AND_API_UNIT_TESTS_INC_XDYNFORMETEST_HPP_ */
