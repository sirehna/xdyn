/*
 * DiffractionInterpolatorTest.hpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cady
 */

#ifndef DIFFRACTIONINTERPOLATORTEST_HPP_
#define DIFFRACTIONINTERPOLATORTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class DiffractionInterpolatorTest : public ::testing::Test
{
    protected:
        DiffractionInterpolatorTest();
        virtual ~DiffractionInterpolatorTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* DIFFRACTIONINTERPOLATORTEST_HPP_ */
