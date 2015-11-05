/*
 * BretschneiderSpectrumTest.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "BretschneiderSpectrumTest.hpp"
#include "BretschneiderSpectrum.hpp"
#include "InvalidInputException.hpp"

#define NB_TRIALS 10

BretschneiderSpectrumTest::BretschneiderSpectrumTest() : a(ssc::random_data_generator::DataGenerator(7779))
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

TEST_F(BretschneiderSpectrumTest, should_throw_if_Hs_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().no().greater_than(0);
        const double Tp = a.random<double>().greater_than(0);
        ASSERT_THROW(BretschneiderSpectrum(Hs, Tp),InvalidInputException);
    }
}

TEST_F(BretschneiderSpectrumTest, should_throw_if_Tp_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = a.random<double>().no().greater_than(0);
        ASSERT_THROW(BretschneiderSpectrum(Hs, Tp),InvalidInputException);
    }
}

TEST_F(BretschneiderSpectrumTest, should_throw_if_Tp_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double Tp = 0;
        ASSERT_THROW(BretschneiderSpectrum(Hs, Tp),InvalidInputException);
    }
}

TEST_F(BretschneiderSpectrumTest, should_not_throw_if_Hs_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = 0;
        const double Tp = a.random<double>().greater_than(0);
        ASSERT_NO_THROW(BretschneiderSpectrum(Hs, Tp));
    }
}

TEST_F(BretschneiderSpectrumTest, should_throw_if_omega_is_negative)
{
    const double Hs = a.random<double>().greater_than(0);
    const double Tp = a.random<double>().greater_than(0);
    const BretschneiderSpectrum S(Hs, Tp);
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double omega = a.random<double>().no().greater_than(0);
        ASSERT_THROW(S(omega),InvalidInputException);
    }
}

TEST_F(BretschneiderSpectrumTest, should_throw_if_omega_is_zero)
{
    const double Hs = a.random<double>().greater_than(0);
    const double Tp = a.random<double>().greater_than(0);
    const BretschneiderSpectrum S(Hs, Tp);
    ASSERT_THROW(S(0),InvalidInputException);
}
