#ifndef WEBSOCKETOBSERVERTEST_HPP_
#define WEBSOCKETOBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class WebSocketObserverTest : public ::testing::Test
{
    protected:
        WebSocketObserverTest();
        virtual ~WebSocketObserverTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
        ssc::random_data_generator::DataGenerator a;
};

#endif
