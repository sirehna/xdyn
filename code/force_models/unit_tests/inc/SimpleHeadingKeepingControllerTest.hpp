/*
 * SimpleTrackKeepingControllerTest.hpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */


#ifndef SIMPLETRACKKEEPINGCONTROLLERTEST_HPP_
#define SIMPLETRACKKEEPINGCONTROLLERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class SimpleHeadingKeepingControllerTest : public ::testing::Test
{
    protected:
        SimpleHeadingKeepingControllerTest();
        virtual ~SimpleHeadingKeepingControllerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* SIMPLETRACKKEEPINGCONTROLLERTEST_HPP_ */
