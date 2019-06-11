/*
 * FroudeKrylovModelTest.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: cady
 */

#include "FroudeKrylovForceModelTest.hpp"
#include "BodyWithSurfaceForces.hpp"
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
#include "env_for_tests.hpp"
#include "YamlWaveModelInput.hpp"
#include "Stretching.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

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
    YamlStretching ys;
    ys.h = 0;
    ys.delta = 1;
    const Stretching ss(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi0), omega_min, omega_max, nfreq, ss);
    int random_seed = 0;
    return TR1(shared_ptr)<WaveModel>(new Airy(A, random_seed));
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

    BodyStates states = get_body(BODY, points)->get_states();
    states.G = ssc::kinematics::Point("NED",0,2,2./3.);
    BodyPtr body(new BodyWithSurfaceForces(states, 0, BlockedDOF("")));

    FroudeKrylovForceModel F(BODY, env);
    ASSERT_EQ("non-linear Froude-Krylov", F.model_name());
    const double t = 0;
    body->update_intersection_with_free_surface(env, t);
    const ssc::kinematics::Wrench Ffk = F(body->get_states(), t);
//! [FroudeKrylovForceModelTest example]
//! [FroudeKrylovForceModelTest expected output]
    ASSERT_DOUBLE_EQ(-11056.734651002685, Ffk.X());
    ASSERT_DOUBLE_EQ(0, Ffk.Y());
    ASSERT_DOUBLE_EQ(0, Ffk.Z());
    ASSERT_DOUBLE_EQ(0, Ffk.K());
    ASSERT_DOUBLE_EQ(-3910.495427875187, Ffk.M());
    ASSERT_DOUBLE_EQ(-432.07086885338083, Ffk.N());
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
    YamlStretching ys;
    ys.h = 0;
    ys.delta = 1;
    const Stretching ss(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq, ss);
    const Airy wave(A, phi);

    const std::vector<double> x{-0.1, 0.1, 0, 0, 0, 0};
    const std::vector<double> y{0, 0, -0.1, 0.1, 0, 0};
    const std::vector<double> z{0.2, 0.2, 0.2, 0.2, 0.1, 0.3};
    const std::vector<double> eta = wave.get_elevation(x,y,t);
    
    const double F1 = rho*g*dS*Hs/2*exp(-k*(z.at(0)-eta.at(0)))*cos(omega0*t-k*(x.at(0)*cos(psi)+y.at(0)*sin(psi))+phi);
    const double F2 = rho*g*dS*Hs/2*exp(-k*(z.at(1)-eta.at(1)))*cos(omega0*t-k*(x.at(1)*cos(psi)+y.at(1)*sin(psi))+phi);
    const double F3 = rho*g*dS*Hs/2*exp(-k*(z.at(2)-eta.at(2)))*cos(omega0*t-k*(x.at(2)*cos(psi)+y.at(2)*sin(psi))+phi);
    const double F4 = rho*g*dS*Hs/2*exp(-k*(z.at(3)-eta.at(3)))*cos(omega0*t-k*(x.at(3)*cos(psi)+y.at(3)*sin(psi))+phi);
    const double F5 = rho*g*dS*Hs/2*exp(-k*(z.at(4)-eta.at(4)))*cos(omega0*t-k*(x.at(4)*cos(psi)+y.at(4)*sin(psi))+phi);
    const double F6 = rho*g*dS*Hs/2*exp(-k*(z.at(5)-eta.at(5)))*cos(omega0*t-k*(x.at(5)*cos(psi)+y.at(5)*sin(psi))+phi);

    const double Fx = F2 - F1;
    const double Fy = F4 - F3;
    const double Fz = F6 - F5;

    EXPECT_NEAR(-0.28057, Fx, BIG_EPS);
    EXPECT_NEAR(0, Fy, BIG_EPS);
    EXPECT_NEAR(-0.56479215074847744, Fz, BIG_EPS);
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
    YamlStretching ys;
    ys.h = 0;
    ys.delta = 1;
    const Stretching ss(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq, ss);

    const EnvironmentAndFrames env = get_environment_and_frames(TR1(shared_ptr)<WaveModel>(new Airy(A, phi)));

    BodyStates states = get_body(BODY, cube(0.2,0,0,0.2))->get_states();
    states.G = ssc::kinematics::Point("NED",0,0,0.2);
    BodyPtr body(new BodyWithSurfaceForces(states,0,BlockedDOF("")));

    FroudeKrylovForceModel F(BODY, env);
    body->update_intersection_with_free_surface(env, t);
    const ssc::kinematics::Wrench Ffk = F(states, t);
    ASSERT_NEAR(-0.56219471494913797, Ffk.X(), EPS);
    ASSERT_NEAR(0, Ffk.Y(), EPS);
    ASSERT_NEAR(-0.27603603957852307, Ffk.Z(), EPS);
    ASSERT_NEAR(0, Ffk.K(), EPS);
    ASSERT_NEAR(0, Ffk.M(), EPS);
    ASSERT_NEAR(0, Ffk.N(), EPS);
}
