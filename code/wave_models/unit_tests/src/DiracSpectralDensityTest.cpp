/*
 * DiracSpectralDensityTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiracSpectralDensityTest.hpp"
#include "DiracSpectralDensity.hpp"

DiracSpectralDensityTest::DiracSpectralDensityTest() : a(DataGenerator(2))
{
}

DiracSpectralDensityTest::~DiracSpectralDensityTest()
{
}

void DiracSpectralDensityTest::SetUp()
{
}

void DiracSpectralDensityTest::TearDown()
{
}

TEST_F(DiracSpectralDensityTest, example)
{
//! [DiracSpectralDensityTest example]
    const double omega0 = a.random<double>();
    const double Hs = a.random<double>();
    const DiracSpectralDensity D(omega0,Hs);
//! [DiracSpectralDensityTest example]
//! [DiracSpectralDensityTest expected output]
    ASSERT_DOUBLE_EQ(Hs, D(omega0));
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double omega = a.random<double>().but_not(omega0);
        ASSERT_EQ(0, D(omega));
    }
//! [DiracSpectralDensityTest expected output]
}


