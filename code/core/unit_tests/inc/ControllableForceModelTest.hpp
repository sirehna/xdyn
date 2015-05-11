/*
 * ControllableForceModelTest.hpp
 *
 *  Created on: May 11, 2015
 *      Author: cady
 */


#ifndef CONTROLLABLEFORCEMODELTEST_HPP_
#define CONTROLLABLEFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class ControllableForceModelTest : public ::testing::Test
{
    protected:
        ControllableForceModelTest();
        virtual ~ControllableForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* CONTROLLABLEFORCEMODELTEST_HPP_ */
