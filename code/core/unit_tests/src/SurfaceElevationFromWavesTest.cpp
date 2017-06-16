/*
 * WaveModelFromSpectraTest.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "discretize.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include <ssc/kinematics.hpp>
#include "SurfaceElevationBuilder.hpp"
#include "SurfaceElevationFromWavesTest.hpp"
#include "SurfaceElevationFromWaves.hpp"
#include "YamlWaveModelInput.hpp"
#include "YamlWaveModelInput.hpp"
#include "Stretching.hpp"
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#include "InvalidInputException.hpp"

SurfaceElevationFromWavesTest::SurfaceElevationFromWavesTest() : a(ssc::random_data_generator::DataGenerator(45454))
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

TR1(shared_ptr)<WaveModel> SurfaceElevationFromWavesTest::get_model(const double Hs, const double Tp, const double phase, const double water_depth) const
{
    const double psi0 = 0;
    if (Tp==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Tp is zero (which implies omega0 will be infinite)");
    }
    const double omega0 = 2*PI/Tp;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    YamlStretching y;
    y.h = 0;
    y.delta = 1;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, 1, water_depth, s);
    return TR1(shared_ptr)<WaveModel>(new Airy(A, phase));
}

TR1(shared_ptr)<WaveModel> SurfaceElevationFromWavesTest::get_model(const size_t nfreq) const
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    YamlStretching y;
    y.h = 0;
    y.delta = 1;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq, s);
    int random_seed = 0;
    return TR1(shared_ptr)<WaveModel>(new Airy(A, random_seed));
}

TR1(shared_ptr)<WaveModel> SurfaceElevationFromWavesTest::get_model() const
{
    return get_model(a.random<size_t>().between(2,100));
}

TEST_F(SurfaceElevationFromWavesTest, default_constructor_contains_an_empty_output_wave_mesh)
{
    const auto ms = SurfaceElevationFromWaves(get_model()).get_output_mesh_size();
    ASSERT_EQ(0,ms.first);
    ASSERT_EQ(0,ms.second);
}

TEST_F(SurfaceElevationFromWavesTest, relative_wave_height)
{
//! [SurfaceElevationFromWavesTest relative_wave_height example]
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double k_ = 4.*PI*PI/Tp/Tp/9.81;
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    SurfaceElevationFromWaves wave(get_model());
    const double phi = 3.4482969340598712549;
//! [SurfaceElevationFromWavesTest relative_wave_height example]
//! [SurfaceElevationFromWavesTest relative_wave_height expected output]
    for (double t = 0 ; t < 30 ; t+=0.1)
    {
        const ssc::kinematics::Point P("NED", a.random<double>(), a.random<double>(), a.random<double>());
        const double x = P.x();
        const double y = P.y();
        ASSERT_NEAR(-Hs/2*sin(-2*PI/Tp*t + k_*(x*cos(psi0)+y*sin(psi0)) +phi), wave.wave_height(x, y, t), 1E-5);
    }
//! [SurfaceElevationFromWavesTest relative_wave_height expected output]
}

TEST_F(SurfaceElevationFromWavesTest, dynamic_pressure)
{
//! [SurfaceElevationFromWavesTest dynamic_pressure example]
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double k_ = 4.*PI*PI/Tp/Tp/9.81;
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    SurfaceElevationFromWaves wave(get_model());
    const double phi = 3.4482969340598712549;
//! [SurfaceElevationFromWavesTest dynamic_pressure example]
//! [SurfaceElevationFromWavesTest dynamic_pressure expected output]
    for (double t = 0 ; t < 30 ; t+=0.1)
    {
        const ssc::kinematics::Point P("NED", a.random<double>().between(-100,100), a.random<double>().between(-100,100), 6);
        const double rho = a.random<double>().between(0,100);
        const double g = a.random<double>().between(0,10);
        const double x = P.x();
        const double y = P.y();
        const double z = P.z();

        ASSERT_NEAR(Hs/2*rho*g*exp(-k_*z)*sin(-2*PI/Tp*t + k_*(x*cos(psi0)+y*sin(psi0)) +phi), wave.get_dynamic_pressure(rho, g, P, k, 0, t), 1E-6) << "t = " << t;
    }
//! [SurfaceElevationFromWavesTest dynamic_pressure expected output]
}

TEST_F(SurfaceElevationFromWavesTest, orbital_velocity)
{
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    SurfaceElevationFromWaves wave(get_model());
    const double g = 4.57;
    const double x = 3.7;
    const double y = 5.3;
    const double z = 1.62;
    const double t = 664.2;
    ssc::kinematics::Point v = wave.orbital_velocity(g, x, y, z, t, 0);
    EXPECT_NEAR(-0.12705396141751218, v.x(),1e-10);
    EXPECT_NEAR(-0.12705396141751218, v.y(),1e-10);
    EXPECT_NEAR(0.37002483136735936, v.z(),1e-10);
}

TEST_F(SurfaceElevationFromWavesTest, bug_detected_by_FS)
{
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    YamlWaveOutput out;
    out.frame_of_reference = "NED";
    out.xmin = -10;
    out.xmax = 10;
    out.nx = 50;
    out.ymin = -100;
    out.ymax = 100;
    out.ny = 20;
    const auto output_mesh = SurfaceElevationBuilderInterface::make_wave_mesh(out);
    SurfaceElevationFromWaves wave(get_model(5), std::make_pair(50, 20), output_mesh);
    SurfaceElevationGrid grid = wave.get_waves_on_mesh_as_a_grid(k, 0);
    ASSERT_EQ(50, grid.x.count());
    ASSERT_EQ(20, grid.y.count());
    ASSERT_EQ(20, grid.z.cols());
    ASSERT_EQ(50, grid.z.rows());

    const double EPS = 1E-6;
    for (size_t i = 0 ; i < 1 ; ++i)
    {
        EXPECT_NEAR((double)grid.z(i,0),-0.46917699300305094,EPS);
        EXPECT_NEAR((double)grid.z(i,1),-0.86869551138231271,EPS);
        EXPECT_NEAR((double)grid.z(i,2),-1.1908530054573996,EPS);
        EXPECT_NEAR((double)grid.z(i,3),-1.406959982302008,EPS);
        EXPECT_NEAR((double)grid.z(i,4),-1.4977711979049479,EPS);
        EXPECT_NEAR((double)grid.z(i,5),-1.4551995278764878,EPS);
        EXPECT_NEAR((double)grid.z(i,6),-1.2830361602181635,EPS);
    }
}

TEST_F(SurfaceElevationFromWavesTest, dynamic_pressure_plus_hydrostatic)
{
    const double psi0 = 0;
    const double Hs = 2;
    const double k_ = 1;
    const double g = 9.81;
    const double rho = 1024;
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    const double h = 20;
    const double omega0 = sqrt(g*k_*tanh(k_*h));
    const double phi = 0;//-PI;
    SurfaceElevationFromWaves wave(get_model(Hs, 2*PI/omega0, phi, h));

    const double t = 0;
    const double EPS = 1E-4;

    const ssc::kinematics::Point P1("NED", PI/2, 0, -1);
    const double phs1 = rho*g*P1.z();
    const double pdyn1 = wave.get_dynamic_pressure(rho, g, P1, k, wave.wave_height(P1.x(), P1.y(), t), t);
    ASSERT_NEAR(-rho*g, phs1, EPS);
    ASSERT_NEAR(rho*g*(cosh(h+1)/cosh(h)), pdyn1, EPS);
    ASSERT_NEAR(rho*g*(cosh(h+1)/cosh(h) - 1), phs1 + pdyn1, EPS);

    const ssc::kinematics::Point P2("NED", PI/2, 0, -0.5);
    const double phs2 = rho*g*P2.z();
    const double pdyn2 = wave.get_dynamic_pressure(rho, g, P2, k, wave.wave_height(P2.x(), P2.y(), t), t);
    ASSERT_NEAR(-rho*g*0.5, phs2, EPS);
    ASSERT_NEAR(rho*g*(cosh(h+0.5)/cosh(h)), pdyn2, EPS);
    ASSERT_NEAR(rho*g*(cosh(h+0.5)/cosh(h) - 0.5), phs2 + pdyn2, EPS);

    const ssc::kinematics::Point P3("NED", PI/2, 0, 0);
    const double phs3 = rho*g*P3.z();
    const double pdyn3 = wave.get_dynamic_pressure(rho, g, P3, k, wave.wave_height(P3.x(), P3.y(), t), t);
    ASSERT_NEAR(0, phs3, EPS);
    ASSERT_NEAR(rho*g, pdyn3, EPS);
    ASSERT_NEAR(rho*g, phs3 + pdyn3, EPS);

    const ssc::kinematics::Point P4("NED", PI, 0, 0);
    const double phs4 = rho*g*P4.z();
    const double pdyn4 = wave.get_dynamic_pressure(rho, g, P4, k, wave.wave_height(P4.x(), P4.y(), t), t);
    ASSERT_NEAR(0, phs4, EPS);
    ASSERT_NEAR(0, pdyn4, EPS);
    ASSERT_NEAR(0, phs4 + pdyn4, EPS);

    const ssc::kinematics::Point P5("NED", 3*PI/2, 0, 1+1E-10);
    const double phs5 = rho*g*P5.z();
    const double pdyn5 = wave.get_dynamic_pressure(rho, g, P5, k, wave.wave_height(P5.x(), P5.y(), t), t);
    ASSERT_NEAR(rho*g, phs5, EPS);
    ASSERT_NEAR(-rho*g*(cosh(h-1)/cosh(h)), pdyn5, EPS);
    ASSERT_NEAR(rho*g*(1-cosh(h-1)/cosh(h)), phs5 + pdyn5, EPS);
}
