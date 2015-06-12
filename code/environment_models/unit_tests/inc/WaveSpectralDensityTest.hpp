/*
 * WaveSpectralDensityTest.hpp
 *
 *  Created on: Jun 12, 2015
 *      Author: cady
 */


#ifndef WAVESPECTRALDENSITYTEST_HPP_
#define WAVESPECTRALDENSITYTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class WaveSpectralDensityTest : public ::testing::Test
{
    protected:
        WaveSpectralDensityTest();
        virtual ~WaveSpectralDensityTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* WAVESPECTRALDENSITYTEST_HPP_ */
