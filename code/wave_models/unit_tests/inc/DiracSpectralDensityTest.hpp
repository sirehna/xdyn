/*
 * DiracSpectralDensityTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DIRACSPECTRALDENSITYTEST_HPP_
#define DIRACSPECTRALDENSITYTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DiracSpectralDensityTest : public ::testing::Test
{
    protected:
        DiracSpectralDensityTest();
        virtual ~DiracSpectralDensityTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* DIRACSPECTRALDENSITYTEST_HPP_ */
