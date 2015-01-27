/*
 * ManeuveringDataSourceBuilderTest.hpp
 *
 *  Created on: Jan 27, 2015
 *      Author: cady
 */


#ifndef MANEUVERINGDATASOURCEBUILDERTEST_HPP_
#define MANEUVERINGDATASOURCEBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class maneuvering_DataSource_builderTest : public ::testing::Test
{
    protected:
        maneuvering_DataSource_builderTest();
        virtual ~maneuvering_DataSource_builderTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* MANEUVERINGDATASOURCEBUILDERTEST_HPP_ */
