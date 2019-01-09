/*
 * SimTest.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMTEST_HPP_
#define SIMTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "GeometricTypes3d.hpp"

class SimTest : public ::testing::Test
{
    protected:
        SimTest();
        virtual ~SimTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        static const VectorOfVectorOfPoints test_ship_stl;
};

#endif  /* SIMTEST_HPP_ */
