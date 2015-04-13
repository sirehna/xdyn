/*
 * MapObserverTest.hpp
 *
 *  Created on: Apr 13, 2015
 *      Author: cady
 */


#ifndef MAPOBSERVERTEST_HPP_
#define MAPOBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class MapObserverTest : public ::testing::Test
{
    protected:
        MapObserverTest();
        virtual ~MapObserverTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* MAPOBSERVERTEST_HPP_ */
