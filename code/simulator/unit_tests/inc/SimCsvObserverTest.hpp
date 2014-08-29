/*
 * SimCsvObserverTest.hpp
 *
 *  Created on: Aug 28, 2014
 *      Author: cady
 */

#ifndef SIMCSVOBSERVERTEST_HPP_
#define SIMCSVOBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class SimCsvObserverTest : public ::testing::Test
{
    protected:
        SimCsvObserverTest();
        virtual ~SimCsvObserverTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* SIMCSVOBSERVERTEST_HPP_ */
