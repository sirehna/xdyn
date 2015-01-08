/*
 * test_hs.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "TriMeshTestData.hpp"
#include "MeshIntersector.hpp"
#include "YamlRotation.hpp"
#include "BodyBuilder.hpp"
#include "GeometricTypes3d.hpp"
#include <ssc/kinematics.hpp>
#include "generate_anthineas.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "discretize.hpp"
#include "Airy.hpp"
#include <ssc/macros.hpp>
#include "SurfaceElevationFromWaves.hpp"

#define BODY "body 1"

#define N 2000
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

Body get_body(const std::string& name);
Body get_body(const std::string& name, const VectorOfVectorOfPoints& points);

Body get_body(const std::string& name)
{
    return get_body(name, two_triangles());
}

Body get_body(const std::string& name, const VectorOfVectorOfPoints& points)

{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, points, 0);
}

TR1(shared_ptr)<WaveModel> get_wave_model();
TR1(shared_ptr)<WaveModel> get_wave_model()
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

EnvironmentAndFrames get_env();
EnvironmentAndFrames get_env()
{
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    const ssc::kinematics::Point G("NED",0,2,2./3.);
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), "mesh(" BODY ")"));
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    TR1(shared_ptr)<ssc::kinematics::PointMatrix> mesh;
    //env.w = SurfaceElevationPtr(new DefaultSurfaceElevation(0, mesh));
    env.w = SurfaceElevationPtr(new SurfaceElevationFromWaves(get_wave_model()));
    return env;
}

void test(const ForceModel& F, const EnvironmentAndFrames& env, const size_t n);
void test(const ForceModel& F, const EnvironmentAndFrames& env, size_t n)
{
    Body body = get_body(BODY, anthineas());
    const double t = 0;
    body.update_intersection_with_free_surface(env, t);
    ssc::kinematics::Wrench Fhs;
    for (size_t i = 0 ; i < n ; ++i) F(body.states, t);
}

int main(int argc, char* argv[])
{
    const size_t n = argc>1 ? (size_t)atoi(argv[1]) : N;
    auto env = get_env();
    test(FroudeKrylovForceModel(env), env, n);
    //test(FastHydrostaticForceModel(env), env, N);
    return 0;
}
