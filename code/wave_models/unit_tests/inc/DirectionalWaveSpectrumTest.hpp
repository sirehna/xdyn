/*
 * DirectionalWaveSpectrumTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DIRECTIONALWAVESPECTRUMTEST_HPP_
#define DIRECTIONALWAVESPECTRUMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DirectionalWaveSpectrumTest : public ::testing::Test
{
    protected:
        DirectionalWaveSpectrumTest();
        virtual ~DirectionalWaveSpectrumTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* DIRECTIONALWAVESPECTRUMTEST_HPP_ */
