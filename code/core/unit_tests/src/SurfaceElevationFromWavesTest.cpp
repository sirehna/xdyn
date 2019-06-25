/*
 * SurfaceElevationFromWavesTest.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "SurfaceElevationFromWavesTest.hpp"
#include "discretize.hpp"
#include "Airy.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "SurfaceElevationBuilder.hpp"
#include "SurfaceElevationFromWaves.hpp"
#include "YamlWaveModelInput.hpp"
#include "YamlWaveModelInput.hpp"
#include "Stretching.hpp"
#include "InvalidInputException.hpp"
#include <ssc/kinematics.hpp>
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

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

WaveModelPtr SurfaceElevationFromWavesTest::get_model(const double psi0, const double Hs, const double Tp, const double phase, const double water_depth, const double omega_min, const double omega_max, const size_t nfreq) const
{
    if (Tp==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Tp is zero (which implies omega0 will be infinite)");
    }
    const double omega0 = 2*PI/Tp;
    YamlStretching y;
    y.h = 0;
    y.delta = 1;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq, water_depth, s);
    return WaveModelPtr(new Airy(A, phase));
}

WaveModelPtr SurfaceElevationFromWavesTest::get_model(const double Hs, const double Tp, const double phase, const double water_depth) const
{
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const double psi0 = 0;
    return get_model(psi0, Hs, Tp, phase, water_depth, omega_min, omega_max, 1);
}

WaveModelPtr SurfaceElevationFromWavesTest::get_model(const size_t nfreq) const
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const double omega0 = 2*PI/Tp;
    YamlStretching y;
    y.h = 0;
    y.delta = 1;
    const Stretching s(y);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq, s);
    int random_seed = 0;
    return WaveModelPtr(new Airy(A, random_seed));
}

WaveModelPtr SurfaceElevationFromWavesTest::get_model() const
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
        const std::vector<double> x{P.x()};
        const std::vector<double> y{P.y()};
        ASSERT_NEAR(-Hs/2*sin(-2*PI/Tp*t + k_*(x.at(0)*cos(psi0)+y.at(0)*sin(psi0)) +phi), wave.get_and_check_wave_height(x, y, t).at(0), 1E-5);
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

        const ssc::kinematics::Point Q("NED", a.random<double>().between(-100, 100), a.random<double>().between(-100, 100), 6);
        ssc::kinematics::PointMatrix P(Q.v, "NED");
        const double rho = a.random<double>().between(0,100);
        const double g = a.random<double>().between(0,10);
        const double x = P.m(0,0);
        const double y = P.m(1,0);
        const double z = P.m(2,0);

        ASSERT_NEAR(Hs/2*rho*g*exp(-k_*z)*sin(-2*PI/Tp*t + k_*(x*cos(psi0)+y*sin(psi0)) +phi), wave.get_dynamic_pressure(rho, g, P, k, std::vector<double>{0}, t).at(0), 1E-6) << "t = " << t;
    }
//! [SurfaceElevationFromWavesTest dynamic_pressure expected output]
}

TEST_F(SurfaceElevationFromWavesTest, orbital_velocity)
{
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    SurfaceElevationFromWaves wave(get_model());
    const double g = 4.57;
    const std::vector<double> x {3.7};
    const std::vector<double> y {5.3};
    const std::vector<double> z {1.62};
    const std::vector<double> eta {0};
    const double t = 664.2;
    ssc::kinematics::PointMatrix v = wave.get_and_check_orbital_velocity(g, x, y, z, t, eta);
    EXPECT_NEAR(0.25451004448140752, v.m(0,0),1e-10);
    EXPECT_NEAR(0.25451004448140752, v.m(1,0),1e-10);
    EXPECT_NEAR(0.1991307825735438, v.m(2,0),1e-10);
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

    const ssc::kinematics::Point P1("NED", PI / 2, 0, -1);
    const ssc::kinematics::Point P2("NED", PI / 2, 0, -0.5);
    const ssc::kinematics::Point P3("NED", PI / 2, 0, 0);
    const ssc::kinematics::Point P4("NED", PI, 0, 0);
    const ssc::kinematics::Point P5("NED", 3 * PI / 2, 0, 1 + 1E-10);

    ssc::kinematics::PointMatrix M("NED", 5);
    M.m(0, 0) = P1.x(); M.m(1, 0) = P1.y(); M.m(2, 0) = P1.z();
    M.m(0, 1) = P2.x(); M.m(1, 1) = P2.y(); M.m(2, 1) = P2.z();
    M.m(0, 2) = P3.x(); M.m(1, 2) = P3.y(); M.m(2, 2) = P3.z();
    M.m(0, 3) = P4.x(); M.m(1, 3) = P4.y(); M.m(2, 3) = P4.z();
    M.m(0, 4) = P5.x(); M.m(1, 4) = P5.y(); M.m(2, 4) = P5.z();

    const std::vector<double> x{P1.x(), P2.x(), P3.x(), P4.x(), P5.x()};
    const std::vector<double> y{P1.y(), P2.y(), P3.y(), P4.y(), P5.y()};
    const double t = 0;

    const std::vector<double> wave_height = wave.get_and_check_wave_height(x, y, t);
    const std::vector<double> pdyn = wave.get_dynamic_pressure(rho, g, M, k, wave_height, t);

    const double EPS = 1E-4;
    const double phs1 = rho*g*P1.z();
    ASSERT_NEAR(-rho*g, phs1, EPS);
    ASSERT_NEAR(rho*g*(cosh(h+1)/cosh(h)), pdyn.at(0), EPS);
    ASSERT_NEAR(rho*g*(cosh(h+1)/cosh(h) - 1), phs1 + pdyn.at(0), EPS);

    const double phs2 = rho*g*P2.z();
    ASSERT_NEAR(-rho*g*0.5, phs2, EPS);
    ASSERT_NEAR(rho*g*(cosh(h+0.5)/cosh(h)), pdyn.at(1), EPS);
    ASSERT_NEAR(rho*g*(cosh(h+0.5)/cosh(h) - 0.5), phs2 + pdyn.at(1), EPS);

    const double phs3 = rho*g*P3.z();
    ASSERT_NEAR(0, phs3, EPS);
    ASSERT_NEAR(rho*g, pdyn.at(2), EPS);
    ASSERT_NEAR(rho*g, phs3 + pdyn.at(2), EPS);

    const double phs4 = rho*g*P4.z();
    ASSERT_NEAR(0, phs4, EPS);
    ASSERT_NEAR(0, pdyn.at(3), EPS);
    ASSERT_NEAR(0, phs4 + pdyn.at(3), EPS);

    const double phs5 = rho*g*P5.z();
    ASSERT_NEAR(rho*g, phs5, EPS);
    ASSERT_NEAR(-rho*g*(cosh(h-1)/cosh(h)), pdyn.at(4), EPS);
    ASSERT_NEAR(rho*g*(1-cosh(h-1)/cosh(h)), phs5 + pdyn.at(4), EPS);
}
