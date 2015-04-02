#ifndef JSONOBSERVERTEST_HPP_
#define JSONOBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class JsonObserverTest : public ::testing::Test
{
    protected:
        JsonObserverTest();
        virtual ~JsonObserverTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
        ssc::random_data_generator::DataGenerator a;
};

#endif
