/*
 * DirectionalWaveSpectrumTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "DirectionalWaveSpectrumTest.hpp"
#include "DirectionalWaveSpectrum.hpp"
#include "JonswapSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"

DirectionalWaveSpectrumTest::DirectionalWaveSpectrumTest() : a(DataGenerator(8421))
{
}

DirectionalWaveSpectrumTest::~DirectionalWaveSpectrumTest()
{
}

void DirectionalWaveSpectrumTest::SetUp()
{
}

void DirectionalWaveSpectrumTest::TearDown()
{
}

TEST_F(DirectionalWaveSpectrumTest, example)
{
//! [DirectionalWaveSpectrumTest example]
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    const DirectionalWaveSpectrum A(S, D, 0.01, 3, 1000);
    const DirectionalWaveSpectrum::Output out = A.get_spectra();
//! [DirectionalWaveSpectrumTest example]
//! [DirectionalWaveSpectrumTest expected output]
    ASSERT_EQ(1000, out.Dj.size());
    ASSERT_EQ(1000, out.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999., A.get_domega());
    ASSERT_DOUBLE_EQ(0.002*PI, A.get_dpsi());
//! [DirectionalWaveSpectrumTest expected output]
}

TEST_F(DirectionalWaveSpectrumTest, Dirac_in_frequency)
{
    const double omega0 = 0.2;
    const double Hs = 3;
    const DiracSpectralDensity S(omega0, Hs);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    const DirectionalWaveSpectrum A(S, D, 0.01, 3, 1000);
    const DirectionalWaveSpectrum::Output out = A.get_spectra();
    ASSERT_EQ(1000, out.Dj.size());
    ASSERT_EQ(1, out.Si.size());
    ASSERT_DOUBLE_EQ(1, A.get_domega());
    ASSERT_DOUBLE_EQ(0.002*PI, A.get_dpsi());
    ASSERT_DOUBLE_EQ(Hs, out.Si.front());
}

TEST_F(DirectionalWaveSpectrumTest, Dirac_in_direction)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const DirectionalWaveSpectrum A(S, D, 0.01, 3, 1000);
    const DirectionalWaveSpectrum::Output out = A.get_spectra();
    ASSERT_EQ(1, out.Dj.size());
    ASSERT_EQ(1000, out.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999, A.get_domega());
    ASSERT_DOUBLE_EQ(1, A.get_dpsi());
    ASSERT_DOUBLE_EQ(1, out.Dj.front());
}

