/*
 * PiersonMoskowitzSpectrumTest.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "PiersonMoskowitzSpectrumTest.hpp"
#include "PiersonMoskowitzSpectrum.hpp"
#include "InvalidInputException.hpp"

#define NB_TRIALS 10

PiersonMoskowitzSpectrumTest::PiersonMoskowitzSpectrumTest() : a(ssc::random_data_generator::DataGenerator(84212))
{
}

PiersonMoskowitzSpectrumTest::~PiersonMoskowitzSpectrumTest()
{
}

void PiersonMoskowitzSpectrumTest::SetUp()
{
}

void PiersonMoskowitzSpectrumTest::TearDown()
{
}

TEST_F(PiersonMoskowitzSpectrumTest, example)
{
//! [PiersonMoskowitzSpectrumTest example]
    const PiersonMoskowitzSpectrum S(10,5.*PI/8.);
//! [PiersonMoskowitzSpectrumTest example]
//! [PiersonMoskowitzSpectrumTest expected output]
    const double eps = 1e-10;
    ASSERT_NEAR(0.004600020635184, S(1.8888888888), eps);
    ASSERT_NEAR(0.179258995153943, S(2.166666666666667), eps);
    ASSERT_NEAR(0.955512136348661, S(2.444444444444445), eps);
    ASSERT_NEAR(2.014925308286375, S(2.722222222222222), eps);
    ASSERT_NEAR(2.673499618095267, S(3), 1E-3);
//! [PiersonMoskowitzSpectrumTest expected output]
}

TEST_F(PiersonMoskowitzSpectrumTest, Jonswap_reduces_to_Pierson_Moskowitz_for_gamma_equals_1_)
{
    JonswapSpectrum Sjonswap(4, 8, 1);
    const PiersonMoskowitzSpectrum Spierson_moskowitz(4, 8);
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double omega = a.random<double>().between(0.01,3);
        ASSERT_DOUBLE_EQ(Sjonswap(omega), Spierson_moskowitz(omega));
    }
}

TEST_F(PiersonMoskowitzSpectrumTest, should_throw_if_Hs_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().no().greater_than(0);
        const double Tp = a.random<double>().greater_than(0);
        ASSERT_THROW(PiersonMoskowitzSpectrum(Hs, Tp),InvalidInputException);
    }
}

TEST_F(PiersonMoskowitzSpectrumTest, should_throw_if_Tp_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = a.random<double>().no().greater_than(0);
        ASSERT_THROW(PiersonMoskowitzSpectrum(Hs, Tp),InvalidInputException);
    }
}

TEST_F(PiersonMoskowitzSpectrumTest, should_throw_if_Tp_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = 0;
        ASSERT_THROW(PiersonMoskowitzSpectrum(Hs, Tp),InvalidInputException);
    }
}

TEST_F(PiersonMoskowitzSpectrumTest, should_not_throw_if_Hs_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = 0;
        const double Tp = a.random<double>().greater_than(0);
        ASSERT_NO_THROW(PiersonMoskowitzSpectrum(Hs, Tp));
    }
}

TEST_F(PiersonMoskowitzSpectrumTest, should_throw_if_omega_is_negative)
{
    const double Hs = a.random<double>().greater_than(0);
    const double Tp = a.random<double>().greater_than(0);
    const PiersonMoskowitzSpectrum S(Hs, Tp);
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double omega = a.random<double>().no().greater_than(0);
        ASSERT_THROW(S(omega),InvalidInputException);
    }
}

TEST_F(PiersonMoskowitzSpectrumTest, should_throw_if_omega_is_zero)
{
    const double Hs = a.random<double>().greater_than(0);
    const double Tp = a.random<double>().greater_than(0);
    const PiersonMoskowitzSpectrum S(Hs, Tp);
    ASSERT_THROW(S(0),InvalidInputException);
}
