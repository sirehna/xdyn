/*
 * Cos2sDirectionalSpreadingTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef COS2SDIRECTIONALSPREADINGTEST_HPP_
#define COS2SDIRECTIONALSPREADINGTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class Cos2sDirectionalSpreadingTest : public ::testing::Test
{
    protected:
        Cos2sDirectionalSpreadingTest();
        virtual ~Cos2sDirectionalSpreadingTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* COS2SDIRECTIONALSPREADINGTEST_HPP_ */
