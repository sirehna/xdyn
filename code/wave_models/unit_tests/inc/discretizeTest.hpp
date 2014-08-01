/*
 * DiscretizeTest.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DISCRETIZETEST_HPP_
#define DISCRETIZETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class discretizeTest : public ::testing::Test
{
    protected:
        discretizeTest();
        virtual ~discretizeTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* DISCRETIZETEST_HPP_ */
