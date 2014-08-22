/*
 * WaveModelFromSpectraTest.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "discretize.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "Kinematics.hpp"
#include "Point.hpp"
#include "SurfaceElevationFromWavesTest.hpp"
#include "SurfaceElevationFromWaves.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

SurfaceElevationFromWavesTest::SurfaceElevationFromWavesTest() : a(DataGenerator(45454))
{
}

SurfaceElevationFromWavesTest::~SurfaceElevationFromWavesTest()
{
}

void SurfaceElevationFromWavesTest::SetUp()
{
}

void SurfaceElevationFromWavesTest::TearDown()
{
}

TR1(shared_ptr)<WaveModel> SurfaceElevationFromWavesTest::get_model() const
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    return TR1(shared_ptr)<WaveModel>(new Airy(A, random_seed));
}

/*
 * TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const DefaultWaveModel w(10);
    const Point P("NED", 0, 0, -20);
//! [DefaultWaveModelTest example]
//! [DefaultWaveModelTest expected output]
    ASSERT_DOUBLE_EQ(-30, w.get_relative_wave_height(P, k, a.random<double>()));



    const double x = a.random<double>();
    const double y = a.random<double>();
    const double phi = 0.54881350230425596237;
    const double k = 4.*PI*PI/Tp/Tp/9.81;
    //! [AiryTest example]
    //! [AiryTest expected output]
    for (double t = 0 ; t < 3*Tp ; t+=0.1)
    {
        ASSERT_NEAR(sqrt(2*Hs)*cos(k*(x*cos(psi0)+y*sin(psi0))-2*PI/Tp*t +phi), wave.elevation(x,y,t), 1E-6);
    }

 */

TEST_F(SurfaceElevationFromWavesTest, example)
{
//! [SurfaceElevationFromWavesTest example]
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double k_ = 4.*PI*PI/Tp/Tp/9.81;
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    SurfaceElevationFromWaves wave(get_model());
    const double phi = 0.54881350230425596237;
//! [SurfaceElevationFromWavesTest example]
//! [SurfaceElevationFromWavesTest expected output]
    for (double t = 0 ; t < 30 ; t+=0.1)
    {
        const Point P("NED", a.random<double>(), a.random<double>(), a.random<double>());
        const double x = P.x();
        const double y = P.y();
        const double z = P.z();
        ASSERT_NEAR(z-sqrt(2*Hs)*cos(k_*(x*cos(psi0)+y*sin(psi0))-2*PI/Tp*t +phi), wave.get_relative_wave_height(P, k, t), 1E-5);
    }
//! [SurfaceElevationFromWavesTest expected output]
}



