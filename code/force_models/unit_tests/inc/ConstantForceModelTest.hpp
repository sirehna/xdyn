/*
 * ConstantForceModelTest.hpp
 *
 *  Created on: Sept 7, 2018
 *      Author: cady
 */

#ifndef CONSTANTFORCEMODELTEST_HPP_
#define CONSTANTFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class ConstantForceModelTest : public ::testing::Test
{
    protected:
        ConstantForceModelTest();
        virtual ~ConstantForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* CONSTANTFORCEMODELTEST_HPP_ */
