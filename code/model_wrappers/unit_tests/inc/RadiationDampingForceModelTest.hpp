/*
 * RadiationDampingForceModelTest.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cady
 */


#ifndef RADIATIONDAMPINGFORCEMODELTEST_HPP_
#define RADIATIONDAMPINGFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

#include "HDBParserForTests.hpp"

class RadiationDampingForceModelTest : public ::testing::Test
{
    protected:
        RadiationDampingForceModelTest();
        virtual ~RadiationDampingForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        HDBParserForTests get_test_data() const;

        ssc::random_data_generator::DataGenerator a;
};

#endif  /* RADIATIONDAMPINGFORCEMODELTEST_HPP_ */
