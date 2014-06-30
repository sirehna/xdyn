/*
 * OutputGeneratorTest.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#ifndef OUTPUTGENERATORTEST_HPP_
#define OUTPUTGENERATORTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class OutputGeneratorTest : public ::testing::Test
{
    protected:
        OutputGeneratorTest();
        virtual ~OutputGeneratorTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* OUTPUTGENERATORTEST_HPP_ */
