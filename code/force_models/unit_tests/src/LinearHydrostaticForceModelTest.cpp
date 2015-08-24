/*
 * LinearHydrostaticForceModelTest.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: cady
 */

#include "gmock/gmock.h"
#include "LinearHydrostaticForceModel.hpp"
#include "LinearHydrostaticForceModelTest.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

using namespace testing; // So we can use 'ElementsAre' unqualified

LinearHydrostaticForceModelTest::LinearHydrostaticForceModelTest() : a(ssc::random_data_generator::DataGenerator(8615615))
{
}

LinearHydrostaticForceModelTest::~LinearHydrostaticForceModelTest()
{
}

void LinearHydrostaticForceModelTest::SetUp()
{
}

void LinearHydrostaticForceModelTest::TearDown()
{
}

TEST_F(LinearHydrostaticForceModelTest, parser_test)
{
    const auto input = LinearHydrostaticForceModel::parse(test_data::linear_hydrostatics());
    ASSERT_DOUBLE_EQ(-2, input.z_eq);
    ASSERT_DOUBLE_EQ(1*PI/180., input.theta_eq);
    ASSERT_DOUBLE_EQ(-3*PI/180., input.phi_eq);
    ASSERT_THAT(input.K1, ElementsAre(1,0,0));
    ASSERT_THAT(input.K2, ElementsAre(0,1,0));
    ASSERT_THAT(input.K3, ElementsAre(0,0,1));
    ASSERT_DOUBLE_EQ(10, input.x1);
    ASSERT_DOUBLE_EQ(10, input.x2);
    ASSERT_DOUBLE_EQ(-10, input.x3);
    ASSERT_DOUBLE_EQ(-10, input.x4);
    ASSERT_DOUBLE_EQ(-10, input.y1);
    ASSERT_DOUBLE_EQ(10, input.y2);
    ASSERT_DOUBLE_EQ(-10, input.y3);
    ASSERT_DOUBLE_EQ(10, input.y4);
}



