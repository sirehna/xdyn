/*
 * QuadraticDampingForceModelTest.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#ifndef QUADRATICDAMPINGFORCEMODELTEST_HPP_
#define QUADRATICDAMPINGFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class QuadraticDampingForceModelTest : public ::testing::Test
{
    protected:
    QuadraticDampingForceModelTest();
        virtual ~QuadraticDampingForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* QUADRATICDAMPINGFORCEMODELTEST_HPP_ */
