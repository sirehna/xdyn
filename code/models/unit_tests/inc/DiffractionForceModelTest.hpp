/*
 * DiffractionForceModelTest.hpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */


#ifndef DIFFRACTIONFORCEMODELTEST_HPP_
#define DIFFRACTIONFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class DiffractionForceModelTest : public ::testing::Test
{
    protected:
        DiffractionForceModelTest();
        virtual ~DiffractionForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* DIFFRACTIONFORCEMODELTEST_HPP_ */
