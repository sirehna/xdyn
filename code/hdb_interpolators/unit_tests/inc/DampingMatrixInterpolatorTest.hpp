/*
 * DampingMatrixInterpolatorTest.hpp
 *
 *  Created on: Nov 27, 2014
 *      Author: cady
 */


#ifndef DAMPINGMATRIXINTERPOLATORTEST_HPP_
#define DAMPINGMATRIXINTERPOLATORTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class DampingMatrixInterpolatorTest : public ::testing::Test
{
    protected:
        DampingMatrixInterpolatorTest();
        virtual ~DampingMatrixInterpolatorTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* DAMPINGMATRIXINTERPOLATORTEST_HPP_ */
