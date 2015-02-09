/*
 * ResultantForceComputerTest.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#ifndef RESULTANTFORCECOMPUTERTEST_HPP_
#define RESULTANTFORCECOMPUTERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "Sim.hpp"

class ResultantForceComputerTest : public ::testing::Test
{
    protected:
        ResultantForceComputerTest();
        virtual ~ResultantForceComputerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        static Sim sim;
};

#endif  /* RESULTANTFORCECOMPUTERTEST_HPP_ */
