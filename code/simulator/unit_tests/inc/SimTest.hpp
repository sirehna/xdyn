/*
 * SimTest.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMTEST_HPP_
#define SIMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "GeometricTypes3d.hpp"

class SimTest : public ::testing::Test
{
    protected:
        SimTest();
        virtual ~SimTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;
        static const VectorOfVectorOfPoints anthineas_stl;
};

#endif  /* SIMTEST_HPP_ */
