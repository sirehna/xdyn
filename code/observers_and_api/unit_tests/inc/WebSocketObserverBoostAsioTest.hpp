#ifndef WEBSOCKETOBSERVERBOOSTASIOTEST_HPP_
#define WEBSOCKETOBSERVERBOOSTASIOTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class BoostAsioTest : public ::testing::Test
{
    protected:
        BoostAsioTest();
        virtual ~BoostAsioTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
        ssc::random_data_generator::DataGenerator a;
};

#endif
