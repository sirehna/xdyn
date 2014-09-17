/*
 * DirectionalWaveSpectrumTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "discretizeTest.hpp"
#include "discretize.hpp"
#include "JonswapSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "WaveModelException.hpp"

discretizeTest::discretizeTest() : a(DataGenerator(8421))
{
}

discretizeTest::~discretizeTest()
{
}

void discretizeTest::SetUp()
{
}

void discretizeTest::TearDown()
{
}

TEST_F(discretizeTest, example)
{
//! [discretizeTest example]
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000);
//! [discretizeTest example]
//! [discretizeTest expected output]
    ASSERT_EQ(1000, A.Dj.size());
    ASSERT_EQ(1000, A.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999., A.domega);
    ASSERT_DOUBLE_EQ(0.002*PI, A.dpsi);
//! [discretizeTest expected output]
}

TEST_F(discretizeTest, Dirac_in_frequency)
{
    const double omega0 = 0.2;
    const double Hs = 3;
    const DiracSpectralDensity S(omega0, Hs);
    const Cos2sDirectionalSpreading D(PI/4, 2);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000);
    ASSERT_EQ(1000, A.Dj.size());
    ASSERT_EQ(1, A.Si.size());
    ASSERT_DOUBLE_EQ(1, A.domega);
    ASSERT_DOUBLE_EQ(0.002*PI, A.dpsi);
    ASSERT_DOUBLE_EQ(Hs, A.Si.front());
}

TEST_F(discretizeTest, Dirac_in_direction)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const DiscreteDirectionalWaveSpectrum A = discretize(S, D, 0.01, 3, 1000);
    ASSERT_EQ(1, A.Dj.size());
    ASSERT_EQ(1000, A.Si.size());
    ASSERT_DOUBLE_EQ(2.99/999, A.domega);
    ASSERT_DOUBLE_EQ(1, A.dpsi);
    ASSERT_DOUBLE_EQ(1, A.Dj.front());
}

TEST_F(discretizeTest, should_throw_if_omega_min_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().no().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_omega_min_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = 0;
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_omega_max_is_negative)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(0);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_omega_max_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = 0;
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_omega_max_is_lower_than_omega_min)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().no().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>();
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_nfreq_is_zero)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 0;
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_nfreq_is_one_but_omega_min_is_not_omega_max)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(0);
    const size_t nfreq = 1;
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, should_throw_if_omega_min_equals_omega_max_but_nfreq_is_not_one)
{
    const double Hs = 3;
    const double Tp = 4;
    const double gamma = 1.4;
    const JonswapSpectrum S(Hs, Tp, gamma);
    const DiracDirectionalSpreading D(PI/4);
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = omega_min;
    const size_t nfreq = a.random<size_t>().but_not(1);
    ASSERT_THROW(discretize(S, D, omega_min, omega_max, nfreq), WaveModelException);
}

TEST_F(discretizeTest, flatten)
{
    DiscreteDirectionalWaveSpectrum d;
    d.Si = a.random_vector_of<double>().of_size(3);
    d.Dj = a.random_vector_of<double>().of_size(4);
    d.k = a.random_vector_of<double>().of_size(3);
    d.omega = a.random_vector_of<double>().of_size(3);
    d.phase = std::vector<std::vector<double> >(3,std::vector<double>(4,0));
    d.psi = a.random_vector_of<double>().of_size(4);
    d.domega = a.random<double>();
    d.dpsi = a.random<double>();

    d.omega[0] = 0;
    d.omega[1] = 1;
    d.omega[2] = 2;
    d.psi[0] = 10;
    d.psi[1] = 11;
    d.psi[2] = 12;
    d.psi[3] = 13;

    d.Dj[0] = 1;
    d.Dj[1] = 5;
    d.Dj[2] = 4;
    d.Dj[3] = 3;

    d.Si[0] = 3;
    d.Si[1] = 2;
    d.Si[2] = 4;

    FlatDiscreteDirectionalWaveSpectrum s = flatten(d, 0.15);
    ASSERT_DOUBLE_EQ(d.domega, s.domega);
    ASSERT_DOUBLE_EQ(d.dpsi, s.dpsi);
    ASSERT_EQ(0, s.spectrum.size());

    s = flatten(d, 0.18);
    ASSERT_EQ(1, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(40), s.spectrum[0].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(2, s.spectrum[0].omega);
    ASSERT_DOUBLE_EQ(11, s.spectrum[0].psi);

    s = flatten(d, 0.32);
    ASSERT_EQ(2, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(32), s.spectrum[1].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(2, s.spectrum[1].omega);
    ASSERT_DOUBLE_EQ(12, s.spectrum[1].psi);

    s = flatten(d, 0.44);
    ASSERT_EQ(3, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(30), s.spectrum[2].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(0, s.spectrum[2].omega);
    ASSERT_DOUBLE_EQ(11, s.spectrum[2].psi);

    s = flatten(d, 0.55);
    ASSERT_EQ(4, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(24), s.spectrum[3].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(0, s.spectrum[3].omega);
    ASSERT_DOUBLE_EQ(12, s.spectrum[3].psi);

    s = flatten(d, 0.65);
    ASSERT_EQ(5, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(24), s.spectrum[4].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(2, s.spectrum[4].omega);
    ASSERT_DOUBLE_EQ(13, s.spectrum[4].psi);

    s = flatten(d, 0.74);
    ASSERT_EQ(6, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(20), s.spectrum[5].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(1, s.spectrum[5].omega);
    ASSERT_DOUBLE_EQ(11, s.spectrum[5].psi);

    s = flatten(d, 0.81);
    ASSERT_EQ(7, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(18), s.spectrum[6].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(0, s.spectrum[6].omega);
    ASSERT_DOUBLE_EQ(13, s.spectrum[6].psi);

    s = flatten(d, 0.88);
    ASSERT_EQ(8, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(16), s.spectrum[7].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(1, s.spectrum[7].omega);
    ASSERT_DOUBLE_EQ(12, s.spectrum[7].psi);

    s = flatten(d, 0.93);
    ASSERT_EQ(9, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(12), s.spectrum[8].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(1, s.spectrum[8].omega);
    ASSERT_DOUBLE_EQ(13, s.spectrum[8].psi);

    s = flatten(d, 0.97);
    ASSERT_EQ(10, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(8), s.spectrum[9].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(2, s.spectrum[9].omega);
    ASSERT_DOUBLE_EQ(10, s.spectrum[9].psi);

    s = flatten(d, 0.99);
    ASSERT_EQ(11, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(6), s.spectrum[10].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(0, s.spectrum[10].omega);
    ASSERT_DOUBLE_EQ(10, s.spectrum[10].psi);

    s = flatten(d, 1);
    ASSERT_EQ(12, s.spectrum.size());
    ASSERT_DOUBLE_EQ(sqrt(4), s.spectrum[11].sqrt_2_SiDj);
    ASSERT_DOUBLE_EQ(1, s.spectrum[11].omega);
    ASSERT_DOUBLE_EQ(10, s.spectrum[11].psi);
}
