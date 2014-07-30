/*
 * BretschneiderSpectrumTest.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "BretschneiderSpectrumTest.hpp"
#include "BretschneiderSpectrum.hpp"

BretschneiderSpectrumTest::BretschneiderSpectrumTest() : a(DataGenerator(7779))
{
}

BretschneiderSpectrumTest::~BretschneiderSpectrumTest()
{
}

void BretschneiderSpectrumTest::SetUp()
{
}

void BretschneiderSpectrumTest::TearDown()
{
}

TEST_F(BretschneiderSpectrumTest, example)
{
//! [BretschneiderSpectrumTest example]
    const double EPS = 1e-15;
    ASSERT_NEAR(0.034510725921325,BretschneiderSpectrum(1.0, 5.0)( 1.0),EPS);
    ASSERT_NEAR(0.138042903685298,BretschneiderSpectrum(2.0, 5.0)( 1.0),EPS);
    ASSERT_NEAR(0.000031161194389,BretschneiderSpectrum(2.0, 5.0)(10.0),EPS);
    ASSERT_NEAR(0.276085807370596,BretschneiderSpectrum(2.0,10.0)( 0.5),EPS);
//! [BretschneiderSpectrumTest example]
}

