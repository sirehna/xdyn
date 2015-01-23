/*
 * maneuvering_compilerTest.hpp
 *
 *  Created on: Jan 23, 2015
 *      Author: cady
 */


#ifndef MANEUVERING_COMPILERTEST_HPP_
#define MANEUVERING_COMPILERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class maneuvering_compilerTest : public ::testing::Test
{
    protected:
        maneuvering_compilerTest();
        virtual ~maneuvering_compilerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* MANEUVERING_COMPILERTEST_HPP_ */
