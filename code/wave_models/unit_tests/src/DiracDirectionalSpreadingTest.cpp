/*
 * DiracDirectionalSpreadingTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiracDirectionalSpreadingTest.hpp"
#include "DiracDirectionalSpreading.hpp"

DiracDirectionalSpreadingTest::DiracDirectionalSpreadingTest() : a(ssc::random_data_generator::DataGenerator(58212))
{
}

DiracDirectionalSpreadingTest::~DiracDirectionalSpreadingTest()
{
}

void DiracDirectionalSpreadingTest::SetUp()
{
}

void DiracDirectionalSpreadingTest::TearDown()
{
}

TEST_F(DiracDirectionalSpreadingTest, example)
{
//! [DiracDirectionalSpreadingTest example]
    const double theta0 = a.random<double>();
    const DiracDirectionalSpreading D(theta0);
//! [DiracDirectionalSpreadingTest example]
//! [DiracDirectionalSpreadingTest expected output]
    ASSERT_EQ(1, D(theta0));
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double theta = a.random<double>().but_not(theta0);
        ASSERT_EQ(0, D(theta));
    }
//! [DiracDirectionalSpreadingTest expected output]
}

