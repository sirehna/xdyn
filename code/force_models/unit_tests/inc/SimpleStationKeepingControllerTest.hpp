/*
 * SimpleStationKeepingControllerTest.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: cady
 */


#ifndef SIMPLESTATIONKEEPINGCONTROLLERTEST_HPP_
#define SIMPLESTATIONKEEPINGCONTROLLERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class SimpleStationKeepingControllerTest : public ::testing::Test
{
    protected:
        SimpleStationKeepingControllerTest();
        virtual ~SimpleStationKeepingControllerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* SIMPLESTATIONKEEPINGCONTROLLERTEST_HPP_ */
