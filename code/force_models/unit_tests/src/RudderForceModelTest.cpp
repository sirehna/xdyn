/*
 * RudderForceModelTest.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: cady
 */


#include "RudderForceModelTest.hpp"
#include "RudderForceModel.hpp"

RudderForceModelTest::RudderForceModelTest() : a(ssc::random_data_generator::DataGenerator(21212))
{
}

RudderForceModelTest::~RudderForceModelTest()
{
}

void RudderForceModelTest::SetUp()
{
}

void RudderForceModelTest::TearDown()
{
}

TEST_F(RudderForceModelTest, angle_of_attack)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::InWake riw;
    RudderForceModel::OutsideWake row;
//! [RudderForceModelTest get_alpha_example]
//! [RudderForceModelTest get_alpha_example output]
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double beta = a.random<double>();
        const double alpha = a.random<double>();
        ASSERT_DOUBLE_EQ(riw.get_angle_of_attack(beta, alpha),row.get_angle_of_attack(beta, alpha));
        ASSERT_DOUBLE_EQ(0,riw.get_angle_of_attack(alpha, alpha));
    }
    ASSERT_DOUBLE_EQ(-1,riw.get_angle_of_attack(1, 2));
    ASSERT_DOUBLE_EQ(1,riw.get_angle_of_attack(2, 1));
//! [RudderForceModelTest get_alpha_example output]
}



