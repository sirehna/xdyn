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

EnvironmentAndFrames FroudeKrylovForceModelTest::get_environment_and_frames() const
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    env.w = SurfaceElevationPtr(new SurfaceElevationFromWaves(get_wave_model()));
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
    const EnvironmentAndFrames env = get_environment_and_frames();
    const auto points = get_points();

    Body body = get_body(BODY, points);
    body.G = ssc::kinematics::Point("NED",0,2,2./3.);

    FroudeKrylovForceModel F(env);
    const double t = 0;
    const std::vector<double> vz = env.w->get_relative_wave_height(body.M,env.k,t);
    body.intersector->update_intersection_with_free_surface(vz);
    const ssc::kinematics::Wrench Fhs = F(body, t);
//! [FroudeKrylovForceModelTest example]
//! [FroudeKrylovForceModelTest expected output]
    ASSERT_DOUBLE_EQ(-136170.20578538425616, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(0, Fhs.Z());
    ASSERT_DOUBLE_EQ(0, Fhs.K());
    ASSERT_DOUBLE_EQ(46615.880988761644403, Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
//! [FroudeKrylovForceModelTest expected output]
}
