/*
 * ListOfWaveModelsTest.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "Airy.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "discretize.hpp"
#include "ListOfWaveModelsTest.hpp"
#include "ListOfWaveModels.hpp"

#include "kissfft.hh"
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define EPS 1E-10

ListOfWaveModelsTest::ListOfWaveModelsTest() : a(DataGenerator(88787))
{
}

ListOfWaveModelsTest::~ListOfWaveModelsTest()
{
}

void ListOfWaveModelsTest::SetUp()
{
}

void ListOfWaveModelsTest::TearDown()
{
}

std::pair<std::vector<double>,std::vector<double> > ListOfWaveModelsTest::spectrum(const std::vector<double>& x, const double fs) const
{
    kissfft<double> fft((int)x.size(),false);
    std::vector<std::complex<double> > in(x.size());
    std::vector<std::complex<double> > out(x.size());
    for (size_t i = 0 ; i < x.size() ; ++i)
    {
        in[i] = x[i];
    }
    std::vector<double> f(x.size()), S(x.size());
    fft.transform( &in[0] , &out[0]);
    for (size_t i = 0 ; i < x.size() ; ++i)
    {
        f[i] = ((double)(i+1))*fs/((double)x.size());
        S[i] = abs(out[i]);
    }
    return std::make_pair(f, S);
}

TEST_F(ListOfWaveModelsTest, test_kissfft)
{
    const size_t N = 50;
    const double f = 2;
    const double fs = 10*f;
    const double omega = 2*PI*f;
    const double dt = 1/fs;
    std::vector<double> x(N);
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double ti = (double)i*dt;
        x[i] = cos(omega*ti);
    }

    std::pair<std::vector<double>,std::vector<double> > fft = spectrum(x, fs);

    const size_t n0 = (size_t)(f/fs*double(N));
    for (size_t i = 0 ; i < n0 ; ++i)
    {
        ASSERT_NEAR(0, fft.second.at(i), EPS);
    }
    ASSERT_NEAR(double(N)/2., fft.second.at(n0), EPS);
    for (size_t i = n0+1 ; i < N/2 ; ++i)
    {
        ASSERT_NEAR(0, fft.second.at(i), EPS);
    }
}


TEST_F(ListOfWaveModelsTest, example)
{
//! [ListOfWaveModelsTest example]
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    const TR1(shared_ptr)<Airy> wave(new Airy(A, random_seed));
    const std::vector<TR1(shared_ptr)<WaveModel> > waves(1,wave);
    const ListOfWaveModels l(waves);

//! [ListOfWaveModelsTest example]
//! [ListOfWaveModelsTest expected output]
//! [ListOfWaveModelsTest expected output]
}



