/*
 * DiracSpectralDensityTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiracSpectralDensityTest.hpp"
#include "DiracSpectralDensity.hpp"
#include "InvalidInputException.hpp"

#define NB_TRIALS 10

DiracSpectralDensityTest::DiracSpectralDensityTest() : a(ssc::random_data_generator::DataGenerator(2))
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
    const double omega0 = a.random<double>().between(0,200);
    const double Hs = a.random<double>().between(0,100);
    const DiracSpectralDensity D(omega0,Hs);
//! [DiracSpectralDensityTest example]
//! [DiracSpectralDensityTest expected output]
    ASSERT_DOUBLE_EQ(Hs*Hs/8, D(omega0));
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double omega = a.random<double>().greater_than(0).but_not(omega0);
        ASSERT_EQ(0, D(omega));
    }
//! [DiracSpectralDensityTest expected output]
}

TEST_F(DiracSpectralDensityTest, should_throw_if_Hs_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().no().greater_than(0);
        const double omega0 = a.random<double>().greater_than(0);
        ASSERT_THROW(DiracSpectralDensity(omega0, Hs),InvalidInputException);
    }
}

TEST_F(DiracSpectralDensityTest, should_throw_if_omega0_is_negative)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double omega0 = a.random<double>().no().greater_than(0);
        ASSERT_THROW(DiracSpectralDensity(omega0, Hs),InvalidInputException);
    }
}

TEST_F(DiracSpectralDensityTest, should_throw_if_omega0_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = a.random<double>().greater_than(0);
        const double omega0 = 0;
        ASSERT_THROW(DiracSpectralDensity(omega0, Hs),InvalidInputException);
    }
}

TEST_F(DiracSpectralDensityTest, should_not_throw_if_Hs_is_zero)
{
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double Hs = 0;
        const double omega0 = a.random<double>().greater_than(0);
        ASSERT_NO_THROW(DiracSpectralDensity(omega0, Hs));
    }
}

TEST_F(DiracSpectralDensityTest, should_throw_if_omega_is_negative)
{
    const double Hs = a.random<double>().greater_than(0);
    const double omega0 = a.random<double>().greater_than(0);
    const DiracSpectralDensity S(omega0, Hs);
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        const double omega = a.random<double>().no().greater_than(0);
        ASSERT_THROW(S(omega),InvalidInputException);
    }
}

TEST_F(DiracSpectralDensityTest, should_throw_if_omega_is_zero)
{
    const double Hs = a.random<double>().greater_than(0);
    const double omega0 = a.random<double>().greater_than(0);
    const DiracSpectralDensity S(omega0, Hs);
    ASSERT_THROW(S(0),InvalidInputException);
}
