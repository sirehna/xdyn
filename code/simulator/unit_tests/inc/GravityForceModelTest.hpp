/*
 * GravityForceModelTest.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef GRAVITYFORCEMODELTEST_HPP_
#define GRAVITYFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class GravityForceModelTest : public ::testing::Test
{
    protected:
        GravityForceModelTest();
        virtual ~GravityForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;
};

#endif  /* GRAVITYFORCEMODELTEST_HPP_ */
