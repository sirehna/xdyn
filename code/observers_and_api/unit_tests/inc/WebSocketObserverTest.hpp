#ifndef WEBSOCKETOBSERVERTEST_HPP_
#define WEBSOCKETOBSERVERTEST_HPP_

#include "gtest/gtest.h"

class WebSocketObserverTest : public ::testing::Test
{
    protected:
        WebSocketObserverTest(){};
        virtual ~WebSocketObserverTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
};

#endif
