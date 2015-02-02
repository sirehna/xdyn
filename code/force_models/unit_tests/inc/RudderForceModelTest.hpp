/*
 * RudderForceModelTest.hpp
 *
 *  Created on: Feb 2, 2015
 *      Author: cady
 */


#ifndef RUDDERFORCEMODELTEST_HPP_
#define RUDDERFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class RudderForceModelTest : public ::testing::Test
{
    protected:
        RudderForceModelTest();
        virtual ~RudderForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* RUDDERFORCEMODELTEST_HPP_ */
