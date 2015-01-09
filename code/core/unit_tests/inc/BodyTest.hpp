/*
 * BodyTest.hpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */


#ifndef BODYTEST_HPP_
#define BODYTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class BodyTest : public ::testing::Test
{
    protected:
        BodyTest();
        virtual ~BodyTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        static const BodyPtr body;
};

#endif  /* BODYTEST_HPP_ */
