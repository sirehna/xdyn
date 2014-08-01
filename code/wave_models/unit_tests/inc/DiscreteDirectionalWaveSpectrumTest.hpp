/*
 * DiscreteDirectionalWaveSpectrumTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DISCRETEDIRECTIONALWAVESPECTRUMTEST_HPP_
#define DISCRETEDIRECTIONALWAVESPECTRUMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DiscreteDirectionalWaveSpectrumTest : public ::testing::Test
{
    protected:
    DiscreteDirectionalWaveSpectrumTest();
        virtual ~DiscreteDirectionalWaveSpectrumTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* DISCRETEDIRECTIONALWAVESPECTRUMTEST_HPP_ */
