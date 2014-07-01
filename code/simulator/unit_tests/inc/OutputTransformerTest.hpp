/*
 * OutputTransformerTest.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#ifndef OUTPUTTRANSFORMERRTEST_HPP_
#define OUTPUTTRANSFORMERRTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class OutputTransformerTest : public ::testing::Test
{
    protected:
    OutputTransformerTest();
        virtual ~OutputTransformerTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* OUTPUTTRANSFORMERRTEST_HPP_ */
