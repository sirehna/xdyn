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
#include "WaveModelException.hpp"

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
    const DirectionalWaveSpectrum::Output out = A.get_spectrum();
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
    const DirectionalWaveSpectrum::Output out = A.get_spectrum();
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
    const DirectionalWaveSpectrum::Output out = A.get_spectrum();
    ASSERT_EQ(1, out.Dj.size());
    ASSERT_EQ(1000, out.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999, A.get_domega());
    ASSERT_DOUBLE_EQ(1, A.get_dpsi());
    ASSERT_DOUBLE_EQ(1, out.Dj.front());
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_omega_min_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().no().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_omega_min_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = 0;
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_omega_max_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_omega_max_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = 0;
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_omega_max_is_lower_than_omega_min)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_nfreq_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 0;
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_nfreq_is_one_but_omega_min_is_not_omega_max)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 1;
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DirectionalWaveSpectrumTest, should_throw_if_omega_min_equals_omega_max_but_nfreq_is_not_one)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = omega_min;
    const size_t nfreq = a.random<size_t>().but_not(1);
    ASSERT_THROW(DirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}
