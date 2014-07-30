/*
 * PiersonMoskowitzSpectrumTest.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "PiersonMoskowitzSpectrumTest.hpp"
#include "PiersonMoskowitzSpectrum.hpp"

#define PI (4.*atan(1.))

PiersonMoskowitzSpectrumTest::PiersonMoskowitzSpectrumTest() : a(DataGenerator(84212))
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
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double omega = a.random<double>().between(0.01,3);
        ASSERT_DOUBLE_EQ(Sjonswap(omega), Spierson_moskowitz(omega));
    }
}
