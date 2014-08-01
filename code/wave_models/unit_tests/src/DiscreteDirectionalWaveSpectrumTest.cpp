/*
 * DirectionalWaveSpectrumTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "DiscreteDirectionalWaveSpectrumTest.hpp"
#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "JonswapSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "WaveModelException.hpp"

DiscreteDirectionalWaveSpectrumTest::DiscreteDirectionalWaveSpectrumTest() : a(DataGenerator(8421))
{
}

DiscreteDirectionalWaveSpectrumTest::~DiscreteDirectionalWaveSpectrumTest()
{
}

void DiscreteDirectionalWaveSpectrumTest::SetUp()
{
}

void DiscreteDirectionalWaveSpectrumTest::TearDown()
{
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, example)
{
//! [DiscreteDirectionalWaveSpectrumTest example]
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    const DiscreteDirectionalWaveSpectrum A(S, D, 0.01, 3, 1000);
//! [DiscreteDirectionalWaveSpectrumTest example]
//! [DiscreteDirectionalWaveSpectrumTest expected output]
    ASSERT_EQ(1000, A.Dj.size());
    ASSERT_EQ(1000, A.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999., A.domega);
    ASSERT_DOUBLE_EQ(0.002*PI, A.dpsi);
//! [DiscreteDirectionalWaveSpectrumTest expected output]
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, Dirac_in_frequency)
{
    const double omega0 = 0.2;
    const double Hs = 3;
    const DiracSpectralDensity S(omega0, Hs);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    const DiscreteDirectionalWaveSpectrum A(S, D, 0.01, 3, 1000);
    ASSERT_EQ(1000, A.Dj.size());
    ASSERT_EQ(1, A.Si.size());
    ASSERT_DOUBLE_EQ(1, A.domega);
    ASSERT_DOUBLE_EQ(0.002*PI, A.dpsi);
    ASSERT_DOUBLE_EQ(Hs, A.Si.front());
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, Dirac_in_direction)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const DiscreteDirectionalWaveSpectrum A(S, D, 0.01, 3, 1000);
    ASSERT_EQ(1, A.Dj.size());
    ASSERT_EQ(1000, A.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999, A.domega);
    ASSERT_DOUBLE_EQ(1, A.dpsi);
    ASSERT_DOUBLE_EQ(1, A.Dj.front());
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_omega_min_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().no().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_omega_min_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = 0;
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_omega_max_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_omega_max_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = 0;
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_omega_max_is_lower_than_omega_min)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_nfreq_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 0;
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_nfreq_is_one_but_omega_min_is_not_omega_max)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 1;
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(DiscreteDirectionalWaveSpectrumTest, should_throw_if_omega_min_equals_omega_max_but_nfreq_is_not_one)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = omega_min;
    const size_t nfreq = a.random<size_t>().but_not(1);
    ASSERT_THROW(DiscreteDirectionalWaveSpectrum(S, D, omega_min, omega_max, nfreq), WaveModelException);
}
