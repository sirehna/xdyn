#ifndef SIMHDF5OBSERVERBUILDERTEST_HPP_
#define SIMHDF5OBSERVERBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class SimHdf5ObserverBuilderTest : public ::testing::Test
{
    protected:
        SimHdf5ObserverBuilderTest();
        virtual ~SimHdf5ObserverBuilderTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* SIMHDF5OBSERVERBUILDERTEST_HPP_ */
