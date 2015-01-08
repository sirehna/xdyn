/*
 * LinearDampingForceModelTest.hpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#ifndef LINEARDAMPINGFORCEMODELTEST_HPP_
#define LINEARDAMPINGFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class LinearDampingForceModelTest : public ::testing::Test
{
    protected:
        LinearDampingForceModelTest();
        virtual ~LinearDampingForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* LINEARDAMPINGFORCEMODELTEST_HPP_ */
