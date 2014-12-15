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
    const double omega_min = 0.01;
    const double omega_max = 40;
    const size_t N = 460;
    const auto omegas = RadiationDampingBuilder(TypeOfQuadrature::FILON,TypeOfQuadrature::GAUSS_KRONROD).build_exponential_intervals(omega_min, omega_max, N);
    for (auto omega:omegas) Br.push_back(test_data::analytical_Br(omega));
    return TR1(shared_ptr)<HDBParser>(new HDBParserForTests(omegas, Br));
}

YamlRadiationDamping RadiationDampingForceModelTest::get_yaml_data(const bool show_debug) const
{
    YamlRadiationDamping ret;
    ret.nb_of_points_for_retardation_function_discretization = 50;
    ret.omega_min = 0;
    ret.omega_max = 30;
    ret.output_Br_and_K = show_debug;
    ret.tau_min = 0.2094395;
    ret.tau_max = 10;
    ret.type_of_quadrature_for_convolution = TypeOfQuadrature::CLENSHAW_CURTIS;
    ret.type_of_quadrature_for_cos_transform = TypeOfQuadrature::SIMPSON;
    return ret;
}

TEST_F(RadiationDampingForceModelTest, DISABLED_example)
{
//! [RadiationDampingForceModelTest example]
    RadiationDampingForceModel F(get_hdb_data(),get_yaml_data(false));
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



