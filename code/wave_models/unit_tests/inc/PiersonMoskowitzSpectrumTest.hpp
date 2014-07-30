/*
 * PiersonMoskowitzSpectrumTest.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#ifndef PIERSONMOSKOWITZSPECTRUMTEST_HPP_
#define PIERSONMOSKOWITZSPECTRUMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class PiersonMoskowitzSpectrumTest : public ::testing::Test
{
    protected:
        PiersonMoskowitzSpectrumTest();
        virtual ~PiersonMoskowitzSpectrumTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* PIERSONMOSKOWITZSPECTRUMTEST_HPP_ */
