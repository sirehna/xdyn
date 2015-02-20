/*
 * GZCurveTest.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */


#ifndef GZCURVETEST_HPP_
#define GZCURVETEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

#include "Sim.hpp"

class GZCurveTest : public ::testing::Test
{
    protected:
        GZCurveTest();
        virtual ~GZCurveTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        static Sim sim;
};

#endif  /* GZCURVETEST_HPP_ */
