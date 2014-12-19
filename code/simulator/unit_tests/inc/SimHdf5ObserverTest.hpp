/*
 * SimHdf5ObserverTest.hpp
 *
 *  Created on: Dec 17, 2014
 *      Author: jacquenot
 */

#ifndef SIMHDF5OBSERVERTEST_HPP_
#define SIMHDF5OBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class SimHdf5ObserverTest : public ::testing::Test
{
    protected:
        SimHdf5ObserverTest();
        virtual ~SimHdf5ObserverTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* SIMHDF5OBSERVERTEST_HPP_ */
