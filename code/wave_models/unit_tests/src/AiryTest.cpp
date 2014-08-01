/*
 * AiryTest.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "AiryTest.hpp"
#include "kissfft.hh"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

AiryTest::AiryTest() : a(DataGenerator(54200))
{
}

AiryTest::~AiryTest()
{
}

void AiryTest::SetUp()
{
}

void AiryTest::TearDown()
{
}

#define EPS 1E-10

std::pair<std::vector<double>,std::vector<double> > AiryTest::spectrum(const std::vector<double>& x, const double fs) const
{
    kissfft<double> fft(x.size(),false);
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
        f[i] = (i+1)*fs/x.size();
        S[i] = abs(out[i]);
    }
    return std::make_pair(f, S);
}

TEST_F(AiryTest, test_kissfft)
{
    const size_t N = 50;
    const double f = 2;
    const double fs = 10*f;
    const double omega = 2*PI*f;
    const double dt = 1/fs;
    std::vector<double> x(N);
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double ti = i*dt;
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


