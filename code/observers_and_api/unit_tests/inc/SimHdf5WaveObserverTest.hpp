#ifndef SIMHDF5WAVEOBSERVERTEST_HPP_
#define SIMHDF5WAVEOBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class SimHdf5WaveObserverTest : public ::testing::Test
{
    protected:
        SimHdf5WaveObserverTest();
        virtual ~SimHdf5WaveObserverTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* SIMHDF5WAVEOBSERVERTEST_HPP_ */
