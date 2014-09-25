/*
 * DiracDirectionalSpreadingTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DIRACDIRECTIONALSPREADINGTEST_HPP_
#define DIRACDIRECTIONALSPREADINGTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class DiracDirectionalSpreadingTest : public ::testing::Test
{
    protected:
        DiracDirectionalSpreadingTest();
        virtual ~DiracDirectionalSpreadingTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* DIRACDIRECTIONALSPREADINGTEST_HPP_ */
