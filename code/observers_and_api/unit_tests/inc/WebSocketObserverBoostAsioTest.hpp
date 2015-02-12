#ifndef WEBSOCKETOBSERVERBOOSTASIOTEST_HPP_
#define WEBSOCKETOBSERVERBOOSTASIOTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class WebSocketObserverBoostAsioTest : public ::testing::Test
{
    protected:
        WebSocketObserverBoostAsioTest();
        virtual ~WebSocketObserverBoostAsioTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
        ssc::random_data_generator::DataGenerator a;
};

#endif
