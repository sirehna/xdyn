#ifndef PARSE_ADDRESSTEST_HPP_
#define PARSE_ADDRESSTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class parse_addressTest : public ::testing::Test
{
    protected:
        parse_addressTest();
        virtual ~parse_addressTest(){}
        virtual void SetUp(){}
        virtual void TearDown(){}
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* PARSE_OUTPUTTEST_HPP_ */
