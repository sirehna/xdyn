/*
 * JonswapSpectrumTest.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "JonswapSpectrumTest.hpp"
#include "JonswapSpectrum.hpp"
#include "WaveModelException.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define NB_TRIALS 10

JonswapSpectrumTest::JonswapSpectrumTest() : a(ssc::random_data_generator::DataGenerator(92224))
{
}

JonswapSpectrumTest::~JonswapSpectrumTest()
{
}

void JonswapSpectrumTest::SetUp()
{
}

void JonswapSpectrumTest::TearDown()
{
}

TEST_F(JonswapSpectrumTest, example)
{
//! [JonswapSpectrumTest example]
    const double EPS = 1e-15;
    const double Hs =1.0;
    const double Tp =10.0;
    const double gamma = 3.3;
    const JonswapSpectrum S(Hs, Tp, gamma);
//! [JonswapSpectrumTest example]
//! [JonswapSpectrumTest expected output]
    ASSERT_NEAR(0.001548977510098,S(0.4),EPS);
    ASSERT_NEAR(0.046145520023689,S(0.5),EPS);
    ASSERT_NEAR(0.241661465488933,S(0.6),EPS);
    ASSERT_NEAR(0.144435240716521,S(0.7),EPS);
    ASSERT_NEAR(0.061449169843241,S(0.8),EPS);
//! [JonswapSpectrumTest expected output]
}

TEST_F(JonswapSpectrumTest, spectrum_looks_like_the_one_in_DNV_RPC_C205_fig_3_9)
{
    const double Tp = 8;
    const double Hs = 4;
    const JonswapSpectrum S_gamma_1(Hs, Tp, 1);
    const JonswapSpectrum S_gamma_2(Hs, Tp, 2);
    const JonswapSpectrum S_gamma_5(Hs, Tp, 5);

    ASSERT_LT(1.5, S_gamma_1(0.8));
    ASSERT_LT(S_gamma_1(0.8), 2);
    ASSERT_LT(2.5, S_gamma_2(0.8));
    ASSERT_LT(S_gamma_2(0.8), 3);
    ASSERT_LT(4.5, S_gamma_5(0.8));
    ASSERT_LT(S_gamma_5(0.8), 5);

    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double omega = a.random<double>().between(0,3).but().outside(0.6,1.1);
        ASSERT_LE(S_gamma_1(omega), 1);
        ASSERT_LE(S_gamma_2(omega), 1);
        ASSERT_LE(S_gamma_5(omega), 1);
        ASSERT_LE(S_gamma_2(omega), S_gamma_1(omega));
        ASSERT_LE(S_gamma_5(omega), S_gamma_2(omega));
    }
}

TEST_F(JonswapSpectrumTest, can_compute_the_wave_number_for_infinite_depth)
{
    const double Tp = a.random<double>().greater_than(0);
    const double Hs = a.random<double>().greater_than(0);
    const double gamma = a.random<double>().greater_than(0);
    const JonswapSpectrum S(Hs, Tp, gamma);
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double omega = a.random<double>().greater_than(0);
        ASSERT_DOUBLE_EQ(omega*omega/9.81, S.get_wave_number(omega));
    }
}

TEST_F(JonswapSpectrumTest, can_compute_the_wave_number_for_finite_depth)
{
    const double Tp = a.random<double>().between(1,100);
    const double Hs = a.random<double>().between(1,80);
    const double gamma = a.random<double>().greater_than(0);
    const JonswapSpectrum S(Hs, Tp, gamma);
    for (double h = 1 ; h < 1000 ; h=h+1)
    {
        const double omega = a.random<double>().between(1,2);
        const double k = S.get_wave_number(omega, h);
        ASSERT_DOUBLE_EQ(omega*omega, 9.81*k*tanh(k*h));
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_gamma_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = a.random<double>().greater_than(0);
        const double gamma = a.random<double>().no().greater_than(0);
        ASSERT_THROW(JonswapSpectrum(Hs, Tp, gamma),WaveModelException);
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_gamma_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().no().greater_than(0);
        const double Tp = a.random<double>().greater_than(0);
        const double gamma = 0;
        ASSERT_THROW(JonswapSpectrum(Hs, Tp, gamma),WaveModelException);
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_Hs_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().no().greater_than(0);
        const double Tp = a.random<double>().greater_than(0);
        const double gamma = a.random<double>().greater_than(0);
        ASSERT_THROW(JonswapSpectrum(Hs, Tp, gamma),WaveModelException);
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_Tp_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = a.random<double>().no().greater_than(0);
        const double gamma = a.random<double>().greater_than(0);
        ASSERT_THROW(JonswapSpectrum(Hs, Tp, gamma),WaveModelException);
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_Tp_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = 0;
        const double gamma = a.random<double>().greater_than(0);
        ASSERT_THROW(JonswapSpectrum(Hs, Tp, gamma),WaveModelException);
    }
}

TEST_F(JonswapSpectrumTest, should_not_throw_if_Hs_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = 0;
        const double Tp = a.random<double>().greater_than(0);
        const double gamma = a.random<double>().greater_than(0);
        ASSERT_NO_THROW(JonswapSpectrum(Hs, Tp, gamma));
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_omega_is_negative)
{
    const double Hs = a.random<double>().greater_than(0);
    const double Tp = a.random<double>().greater_than(0);
    const double gamma = a.random<double>().greater_than(0);
    const JonswapSpectrum S(Hs, Tp, gamma);
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double omega = a.random<double>().no().greater_than(0);
        ASSERT_THROW(S(omega),WaveModelException);
    }
}

TEST_F(JonswapSpectrumTest, should_throw_if_omega_is_zero)
{
    const double Hs = a.random<double>().greater_than(0);
    const double Tp = a.random<double>().greater_than(0);
    const double gamma = a.random<double>().greater_than(0);
    const JonswapSpectrum S(Hs, Tp, gamma);
    ASSERT_THROW(S(0),WaveModelException);
}
