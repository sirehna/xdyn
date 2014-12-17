/*
 * RadiationInterpolatorTest.hpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cady
 */

#ifndef RADIATIONINTERPOLATORTEST_HPP_
#define RADIATIONINTERPOLATORTEST_HPP_

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

#endif  /* RADIATIONINTERPOLATORTEST_HPP_ */
