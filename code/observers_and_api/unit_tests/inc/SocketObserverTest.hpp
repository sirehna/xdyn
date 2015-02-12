#ifndef SOCKETOBSERVERTEST_HPP_
#define SOCKETOBSERVERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class SocketObserverTest : public ::testing::Test
{
    protected:
    SocketObserverTest();
        virtual ~SocketObserverTest(){};
        virtual void SetUp(){};
        virtual void TearDown(){};
        ssc::random_data_generator::DataGenerator a;
};

#endif
