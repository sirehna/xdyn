/*
 * FroudeKrylovModelTest.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: cady
 */

#include "FroudeKrylovForceModelTest.hpp"
#include "Body.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "generate_body_for_tests.hpp"
#include "TriMeshTestData.hpp"
#include "MeshIntersector.hpp"
#include <ssc/kinematics.hpp>
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "discretize.hpp"
#include "Airy.hpp"
#include "SurfaceElevationFromWaves.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define BODY "body 1"

#define EPS 1E-8
#define BIG_EPS 1E-3

FroudeKrylovForceModelTest::FroudeKrylovForceModelTest() : a(ssc::random_data_generator::DataGenerator(8511545))
{
}

FroudeKrylovForceModelTest::~FroudeKrylovForceModelTest()
{
}

void FroudeKrylovForceModelTest::SetUp()
{
}

void FroudeKrylovForceModelTest::TearDown()
{
}

TR1(shared_ptr)<WaveModel> FroudeKrylovForceModelTest::get_wave_model() const
{
    const double psi0 = PI/4;
    const double Hs = 3;
    const double Tp = 10;
    const double omega0 = 2*PI/Tp;
    const double omega_min = 0.1;
    const double omega_max = 5;
    const size_t nfreq = 10;
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq);
    int random_seed = 0;
    return TR1(shared_ptr)<WaveModel>(new Airy(A, random_seed));
}

EnvironmentAndFrames FroudeKrylovForceModelTest::get_environment_and_frames(const TR1(shared_ptr)<WaveModel>& wave_model) const
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    env.w = SurfaceElevationPtr(new SurfaceElevationFromWaves(wave_model));
    return env;
}

VectorOfVectorOfPoints FroudeKrylovForceModelTest::get_points() const
{
    auto points = two_triangles();
    for (size_t i = 0 ; i < 2 ; ++i)
    {
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            double x = points[i][j][0];
            points[i][j][0] = points[i][j][2];
            points[i][j][2] = points[i][j][1];
            points[i][j][1] = x;
        }
    }
    return points;
}

TEST_F(FroudeKrylovForceModelTest, example)
{
//! [FroudeKrylovForceModelTest example]
    const EnvironmentAndFrames env = get_environment_and_frames(get_wave_model());
    const auto points = get_points();

    Body body = get_body(BODY, points);
    body.G = ssc::kinematics::Point("NED",0,2,2./3.);

    FroudeKrylovForceModel F(env);
    const double t = 0;
    body.update_intersection_with_free_surface(env, t);
    const ssc::kinematics::Wrench Ffk = F(body, t);
//! [FroudeKrylovForceModelTest example]
//! [FroudeKrylovForceModelTest expected output]
    ASSERT_DOUBLE_EQ(81544.906276859518, Ffk.X());
    ASSERT_DOUBLE_EQ(0, Ffk.Y());
    ASSERT_DOUBLE_EQ(0, Ffk.Z());
    ASSERT_DOUBLE_EQ(0, Ffk.K());
    ASSERT_DOUBLE_EQ(-27915.707583147494, Ffk.M());
    ASSERT_DOUBLE_EQ(0, Ffk.N());
//! [FroudeKrylovForceModelTest expected output]
}

TEST_F(FroudeKrylovForceModelTest, validate_formula_against_sos_stab)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;
    const double g = 9.81;
    const double k = omega0*omega0/g;
    const double dS = 0.2*0.2;
    const double rho = 1025;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);
    const Airy wave(A, phi);

    double x=-0.1; double y=0; double z=0.2;
    const double F1 = rho*g*dS*Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi);
    x=0.1;y=0;z=0.2;
    const double F2 = rho*g*dS*Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi);
    x=0;y=-0.1;z=0.2;
    const double F3 = rho*g*dS*Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi);
    x=0;y=0.1;z=0.2;
    const double F4 = rho*g*dS*Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi);
    x=0;y=0;z=0.1;
    const double F5 = rho*g*dS*Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi);
    x=0;y=0;z=0.3;
    const double F6 = rho*g*dS*Hs/2*exp(-k*(z-wave.elevation(x,y,t)))*cos(omega0*t-k*(x*cos(psi)+y*sin(psi))+phi);

    const double Fx = F2 - F1;
    const double Fy = F4 - F3;
    const double Fz = F6 - F5;

    EXPECT_NEAR(-0.28057, Fx, BIG_EPS);
    EXPECT_NEAR(0, Fy, BIG_EPS);
    EXPECT_NEAR(-0.56744, Fz, BIG_EPS);
}

TEST_F(FroudeKrylovForceModelTest, validation_against_sos_stab)
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq);

    const EnvironmentAndFrames env = get_environment_and_frames(TR1(shared_ptr)<WaveModel>(new Airy(A, phi)));


    Body body = get_body(BODY, cube(0.2,0,0,0.2));
    body.G = ssc::kinematics::Point("NED",0,0,0.2);

    FroudeKrylovForceModel F(env);
    body.update_intersection_with_free_surface(env, t);
    const ssc::kinematics::Wrench Ffk = F(body, t);
    ASSERT_NEAR(-0.28002164687919873, Ffk.X(), EPS);
    ASSERT_NEAR(0, Ffk.Y(), EPS);
    ASSERT_NEAR(-0.56631957463955374, Ffk.Z(), EPS);
    ASSERT_NEAR(0, Ffk.K(), EPS);
    ASSERT_NEAR(0, Ffk.M(), EPS);
    ASSERT_NEAR(0, Ffk.N(), EPS);
}
