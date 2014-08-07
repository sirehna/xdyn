/*
 * WaveModelFromSpectraTest.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef WAVEMODELFROMSPECTRATEST_HPP_
#define WAVEMODELFROMSPECTRATEST_HPP_

#include "gtest/gtest.h"
#include "Airy.hpp"
#include "DataGenerator.hpp"

#include "tr1_macros.hpp"
#include TR1INC(memory)

class WaveModelFromSpectraTest : public ::testing::Test
{
    protected:
        WaveModelFromSpectraTest();
        virtual ~WaveModelFromSpectraTest();
        virtual void SetUp();
        virtual void TearDown();
        TR1(shared_ptr)<WaveModel> get_model() const;
        DataGenerator a;

};

#endif  /* WAVEMODELFROMSPECTRATEST_HPP_ */
