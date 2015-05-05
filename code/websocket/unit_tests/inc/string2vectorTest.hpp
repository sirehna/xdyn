/*
 * vector2stringTest.hpp
 *
 *  Created on: May 5, 2015
 *      Author: cady
 */


#ifndef VECTOR2STRINGTEST_HPP_
#define VECTOR2STRINGTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class string2vectorTest : public ::testing::Test
{
    protected:
        string2vectorTest();
        virtual ~string2vectorTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* VECTOR2STRINGTEST_HPP_ */
