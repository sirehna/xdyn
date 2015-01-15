#ifndef HDF5OBSERVERTEST_HPP_
#define HDF5OBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class Hdf5ObserverTest : public ::testing::Test
{
    protected:
        Hdf5ObserverTest();
        virtual ~Hdf5ObserverTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
        ssc::random_data_generator::DataGenerator a;
};

#endif
