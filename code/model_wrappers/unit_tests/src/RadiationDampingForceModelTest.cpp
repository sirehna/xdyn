/*
 * RadiationDampingForceModelTest.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <ssc/integrate.hpp>

#include "generate_body_for_tests.hpp"
#include "hdb_data.hpp"
#include "RadiationDampingBuilder.hpp"
#include "RadiationDampingForceModel.hpp"
#include "RadiationDampingForceModelTest.hpp"

#define EPS 1E-6

RadiationDampingForceModelTest::RadiationDampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(2121))
{
}

RadiationDampingForceModelTest::~RadiationDampingForceModelTest()
{
}

void RadiationDampingForceModelTest::SetUp()
{
}

void RadiationDampingForceModelTest::TearDown()
{
}

TR1(shared_ptr)<HDBParser> RadiationDampingForceModelTest::get_hdb_data() const
{
    std::vector<double> Br;
    const double omega_min = 0.07;
    const double omega_max = 200;
    const size_t N = 100;
    const auto omegas = RadiationDampingBuilder(TypeOfInterpolation::SPLINES,TypeOfQuadrature::GAUSS_KRONROD).build_exponential_intervals(omega_min, omega_max, N);
    for (auto omega:omegas) Br.push_back(test_data::analytical_Br(omega));
    return TR1(shared_ptr)<HDBParser>(new HDBParserForTests(omegas, Br));
}

YamlRadiationDamping RadiationDampingForceModelTest::get_yaml_data() const
{
    YamlRadiationDamping ret;
    ret.interpolation = TypeOfInterpolation::SPLINES;
    ret.nb_of_points_in_retardation_function = 150;
    ret.quadrature = TypeOfQuadrature::GAUSS_KRONROD;
    return ret;
}

TEST_F(RadiationDampingForceModelTest, DISABLED_convolution_test)
{
    std::vector<double> vBr;
    size_t N = 100;
    const double omega_min = 0.01;
    const double omega_max = 200;
    const double eps = 1E-8;
    RadiationDampingBuilder builder(TypeOfInterpolation::SPLINES, TypeOfQuadrature::SIMPSON);//SIMPSON);
    const auto omegas = builder.build_exponential_intervals(omega_min, omega_max, N);

    for (auto omega:omegas) vBr.push_back(test_data::analytical_Br(omega));
    const auto Br_ = builder.build_interpolator(omegas,vBr);
    auto taus = builder.build_exponential_intervals(2*PI/omegas.back(),2*PI/omegas.front(),N);
    const auto K  = builder.build_retardation_function(Br_,taus,eps,omega_min,omega_max);
    const double Factual_gk = ssc::integrate::GaussKronrod(K).integrate_f(0.031415926535897933936, 89.759790102565503389);
    const double Fexpected_gk = ssc::integrate::GaussKronrod(test_data::analytical_K).integrate_f(0.031415926535897933936, 89.759790102565503389);
//    const double Factual_s = ssc::integrate::Simpson(K).integrate_f(0.031415926535897933936, 89.759790102565503389);
//    const double Fexpected_s = ssc::integrate::Simpson(test_data::analytical_K).integrate_f(0.031415926535897933936, 89.759790102565503389);
//    const double Factual_r = ssc::integrate::Rectangle(K).integrate_f(0.031415926535897933936, 89.759790102565503389);
//    const double Fexpected_r = ssc::integrate::Rectangle(test_data::analytical_K).integrate_f(0.031415926535897933936, 89.759790102565503389);
//    const double Factual_t = ssc::integrate::TrapezoidalIntegration(K).integrate_f(0.031415926535897933936, 89.759790102565503389);
//    const double Fexpected_t = ssc::integrate::TrapezoidalIntegration(test_data::analytical_K).integrate_f(0.031415926535897933936, 89.759790102565503389);

    ASSERT_NEAR(Fexpected_gk, Factual_gk, 1E-3);
/*    COUT(Fexpected_gk);
    COUT(Factual_gk);
    COUT(Fexpected_s);
    COUT(Factual_s);
    COUT(Fexpected_r);
    COUT(Factual_r);
    COUT(Fexpected_t);
    COUT(Factual_t);*/
}

TEST_F(RadiationDampingForceModelTest, DISABLED_example)
{
//! [RadiationDampingForceModelTest example]
    RadiationDampingForceModel F(get_hdb_data(),get_yaml_data());
    const std::string body_name = a.random<std::string>();
    Body b = get_body(body_name);
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest expected output]
    b.u = 1;
    b.v = 1;
    b.w = 1;
    b.p = 1;
    b.q = 1;
    b.r = 1;
    auto Frad = F(b,0);
    ASSERT_EQ(0, Frad.X());
    ASSERT_EQ(0, Frad.Y());
    ASSERT_EQ(0, Frad.Z());
    ASSERT_EQ(0, Frad.K());
    ASSERT_EQ(0, Frad.M());
    ASSERT_EQ(0, Frad.N());
    ASSERT_EQ(body_name, F(b, 0).get_frame());
    b.u = 1;
    b.v = 1;
    b.w = 1;
    b.p = 1;
    b.q = 1;
    b.r = 1;
    Frad = F(b,100);

    //const double Fexpected = 2./PI*ssc::integrate::QuadPack([](const double tau){return exp(-0.1*tau)*(1-tau/100.)*cos(0.5*tau);}).integrate_f(0, 100);

    //const double Fexpected = 2./PI*ssc::integrate::QuadPack(test_data::analytical_K).integrate_f(0.031415926535897933936, 89.759790102565503389);
    const double Fexpected = ssc::integrate::GaussKronrod(test_data::analytical_K).integrate_f(0.031415926535897933936, 89.759790102565503389);
    COUT(Fexpected/Frad.X());
    COUT(Frad.X()/Fexpected);
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Y());
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Z());
    ASSERT_DOUBLE_EQ(Frad.K(),Frad.M());
    ASSERT_DOUBLE_EQ(Frad.M(),Frad.N());
    ASSERT_NEAR(Fexpected, Frad.X(), EPS);
    ASSERT_NEAR(Fexpected, Frad.Y(), EPS);
    ASSERT_NEAR(Fexpected, Frad.Z(), EPS);
    ASSERT_NEAR(Fexpected, Frad.K(), EPS);
    ASSERT_NEAR(Fexpected, Frad.M(), EPS);
    ASSERT_NEAR(Fexpected, Frad.N(), EPS);
//! [RadiationDampingForceModelTest expected output]
}



