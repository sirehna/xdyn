/*
 * Cos2sDirectionalSpreadingTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "Cos2sDirectionalSpreadingTest.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "InvalidInputException.hpp"

#define NB_TRIALS 10

Cos2sDirectionalSpreadingTest::Cos2sDirectionalSpreadingTest() : a(ssc::random_data_generator::DataGenerator(8))
{
}

Cos2sDirectionalSpreadingTest::~Cos2sDirectionalSpreadingTest()
{
}

void Cos2sDirectionalSpreadingTest::SetUp()
{
}

void Cos2sDirectionalSpreadingTest::TearDown()
{
}

TEST_F(Cos2sDirectionalSpreadingTest, example)
{
//! [Cos2sDirectionalSpreadingTest example]
    // Generated with Maple:
    // G:=(theta0,theta,s)->2^(2*s-1)/Pi*GAMMA(s+1)^2/GAMMA(2*s+1)*cos((theta-theta0)/2)^(2*s);
    // G(Pi/4, Pi/3, 3);
    // G(Pi/4, Pi/3, 4);
    // G(Pi/4, Pi/3, 5);
    // G(Pi/5, Pi/6, 1);
    // G(Pi/5, Pi/6, 2);
    const Cos2sDirectionalSpreading G1(PI/4., 3);
    const Cos2sDirectionalSpreading G2(PI/4., 4);
    const Cos2sDirectionalSpreading G3(PI/4., 5);
    const Cos2sDirectionalSpreading G4(PI/5., 1);
    const Cos2sDirectionalSpreading G5(PI/5., 2);
//! [Cos2sDirectionalSpreadingTest example]
//! [Cos2sDirectionalSpreadingTest expected output]
    ASSERT_DOUBLE_EQ(8*pow(cos(PI/24.),6)/(5.*PI),    G1(PI/3.));
    ASSERT_DOUBLE_EQ(64*pow(cos(PI/24.),8)/(35.*PI),  G2(PI/3.));
    ASSERT_DOUBLE_EQ(128*pow(cos(PI/24.),10)/(63.*PI),G3(PI/3.));
    ASSERT_DOUBLE_EQ(pow(cos(PI/60.),2)/(PI),         G4(PI/6.));
    ASSERT_DOUBLE_EQ(4*pow(cos(PI/60.),4)/(3.*PI),    G5(PI/6.));
//! [Cos2sDirectionalSpreadingTest expected output]
}

TEST_F(Cos2sDirectionalSpreadingTest, should_throw_if_s_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        ASSERT_THROW(Cos2sDirectionalSpreading(a.random<double>(), a.random<double>().no().greater_than(0)), InvalidInputException);
    }
}

TEST_F(Cos2sDirectionalSpreadingTest, should_not_throw_if_s_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        ASSERT_NO_THROW(Cos2sDirectionalSpreading(a.random<double>(), 0));
    }
}
