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

#include "BodyStates.hpp"
#include "hdb_data.hpp"
#include "hdb_test.hpp"
#include "RadiationDampingBuilder.hpp"
#include "RadiationDampingForceModel.hpp"
#include "RadiationDampingForceModelTest.hpp"
#include "EnvironmentAndFrames.hpp"
#include "yaml_data.hpp"

#define EPS 5E-2

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

TR1(shared_ptr)<HDBParser> RadiationDampingForceModelTest::get_hdb_data(const bool only_diagonal_terms) const
{
    std::vector<double> Br;
    const double omega_min = 0.01;
    const double omega_max = 40;
    const size_t N = 460;
    const auto omegas = RadiationDampingBuilder(TypeOfQuadrature::FILON,TypeOfQuadrature::GAUSS_KRONROD).build_exponential_intervals(omega_min, omega_max, N);
    for (auto omega:omegas) Br.push_back(test_data::analytical_Br(omega));
    return TR1(shared_ptr)<HDBParser>(new HDBParserForTests(omegas, Br, only_diagonal_terms));
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
    ret.type_of_quadrature_for_convolution = TypeOfQuadrature::SIMPSON;
    ret.type_of_quadrature_for_cos_transform = TypeOfQuadrature::SIMPSON;
    return ret;
}

TEST_F(RadiationDampingForceModelTest, parser)
{
    const YamlRadiationDamping r = RadiationDampingForceModel::parse(test_data::radiation_damping(),false).yaml;
    ASSERT_EQ("test_ship.hdb", r.hdb_filename);
    ASSERT_EQ(50, r.nb_of_points_for_retardation_function_discretization);
    ASSERT_DOUBLE_EQ(30,r.omega_max);
    ASSERT_EQ(0,r.omega_min);
    ASSERT_TRUE(r.output_Br_and_K);
    ASSERT_DOUBLE_EQ(10,r.tau_max);
    ASSERT_DOUBLE_EQ(0.2094395,r.tau_min);
    ASSERT_EQ(TypeOfQuadrature::CLENSHAW_CURTIS, r.type_of_quadrature_for_convolution);
    ASSERT_EQ(TypeOfQuadrature::SIMPSON, r.type_of_quadrature_for_cos_transform);
    ASSERT_DOUBLE_EQ(0.696, r.calculation_point_in_body_frame.x);
    ASSERT_DOUBLE_EQ(0, r.calculation_point_in_body_frame.y);
    ASSERT_DOUBLE_EQ(1.418, r.calculation_point_in_body_frame.z);
}

TEST_F(RadiationDampingForceModelTest, example)
{
//! [RadiationDampingForceModelTest example]
    const auto yaml = get_yaml_data(false);
    RadiationDampingForceModel::Input input;
    input.hdb = get_hdb_data();
    input.yaml = yaml;
    RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_EQ("radiation damping", F.model_name());
    const std::string body_name = a.random<std::string>();
    BodyStates states(100);
    states.name = body_name;
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest expected output]
    states.u.record(0, 1);
    states.v.record(0, 1);
    states.w.record(0, 1);
    states.p.record(0, 1);
    states.q.record(0, 1);
    states.r.record(0, 1);
    auto Frad = F(states,0);
    ASSERT_EQ(0, Frad.X());
    ASSERT_EQ(0, Frad.Y());
    ASSERT_EQ(0, Frad.Z());
    ASSERT_EQ(0, Frad.K());
    ASSERT_EQ(0, Frad.M());
    ASSERT_EQ(0, Frad.N());
    ASSERT_EQ(body_name, F(states, 0).get_frame());
    states.u.record(100, 1);
    states.v.record(100, 1);
    states.w.record(100, 1);
    states.p.record(100, 1);
    states.q.record(100, 1);
    states.r.record(100, 1);
    Frad = F(states,100);

    const double Fexpected = -ssc::integrate::ClenshawCurtisCosine(test_data::analytical_K,0).integrate_f(yaml.tau_min,yaml.tau_max);
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Y());
    ASSERT_DOUBLE_EQ(Frad.X(),Frad.Z());
    ASSERT_DOUBLE_EQ(Frad.K(),Frad.M());
    ASSERT_DOUBLE_EQ(Frad.M(),Frad.N());
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.X(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.Y(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.Z(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.K(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.M(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Fexpected, Frad.N(), EPS);
//! [RadiationDampingForceModelTest expected output]
}

TEST_F(RadiationDampingForceModelTest, should_print_debugging_information_if_required_by_yaml_data)
{
    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(debug.rdbuf());
    ASSERT_TRUE(debug.str().empty());
    // Call the radiation damping model
    RadiationDampingForceModel::Input input;
    input.hdb = get_hdb_data();
    input.yaml = get_yaml_data(true);
    RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_FALSE(debug.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(RadiationDampingForceModelTest, should_not_print_debugging_information_if_not_required_by_yaml_data)
{
    std::stringstream debug;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(debug.rdbuf());
    // Call the radiation damping model
    RadiationDampingForceModel::Input input;
    input.hdb = get_hdb_data();
    input.yaml = get_yaml_data(false);
    RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_TRUE(debug.str().empty());
    // Restore cerr's buffer
    std::cerr.rdbuf(orig);
}

TEST_F(RadiationDampingForceModelTest, force_model_knows_history_length)
{
    RadiationDampingForceModel::Input input;
    input.hdb = get_hdb_data();
    input.yaml = get_yaml_data(false);
    const RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    ASSERT_DOUBLE_EQ(input.yaml.tau_max, F.get_Tmax());
}


TEST_F(RadiationDampingForceModelTest, matrix_product_should_be_done_properly)
{
    RadiationDampingForceModel::Input input;
    const bool only_diagonal_terms = true;
    input.hdb = get_hdb_data(not(only_diagonal_terms));
    input.yaml = get_yaml_data(false);
    input.yaml.type_of_quadrature_for_convolution = TypeOfQuadrature::RECTANGLE;
    const RadiationDampingForceModel F(input, "", EnvironmentAndFrames());
    BodyStates states(100);
    const double tmin = 0.20943950000000000067;
    const double tmax = 10;
    const double eps = 1E-6;
    const double t0 = 5.1047197500000001114;

    states.u.record(0, 0);
    states.v.record(0, 0);
    states.w.record(0, 0);
    states.p.record(0, 0);
    states.q.record(0, 0);
    states.r.record(0, 0);

    states.u.record(tmax-t0-eps, 0);
    states.v.record(tmax-t0-eps, 0);
    states.w.record(tmax-t0-eps, 0);
    states.p.record(tmax-t0-eps, 0);
    states.q.record(tmax-t0-eps, 0);
    states.r.record(tmax-t0-eps, 0);

    const double u0 = 1;
    const double v0 = 2;
    const double w0 = 3;
    const double p0 = 4;
    const double q0 = 5;
    const double r0 = 6;

    states.u.record(tmax-t0-eps/2, u0);
    states.v.record(tmax-t0-eps/2, v0);
    states.w.record(tmax-t0-eps/2, w0);
    states.p.record(tmax-t0-eps/2, p0);
    states.q.record(tmax-t0-eps/2, q0);
    states.r.record(tmax-t0-eps/2, r0);
    states.u.record(tmax-t0+eps/2, u0);
    states.v.record(tmax-t0+eps/2, v0);
    states.w.record(tmax-t0+eps/2, w0);
    states.p.record(tmax-t0+eps/2, p0);
    states.q.record(tmax-t0+eps/2, q0);
    states.r.record(tmax-t0+eps/2, r0);

    states.u.record(tmax-t0+eps, 0);
    states.v.record(tmax-t0+eps, 0);
    states.w.record(tmax-t0+eps, 0);
    states.p.record(tmax-t0+eps, 0);
    states.q.record(tmax-t0+eps, 0);
    states.r.record(tmax-t0+eps, 0);

    states.u.record(tmax, 0);
    states.v.record(tmax, 0);
    states.w.record(tmax, 0);
    states.p.record(tmax, 0);
    states.q.record(tmax, 0);
    states.r.record(tmax, 0);
    const auto Frad = F(states,0);
    const double k = -0.50135576185179109299;
    ASSERT_NEAR(test_data::analytical_K(t0), k, EPS);
    const double conv = -(tmax-tmin )/100*k;


    ASSERT_NEAR(conv * (11*u0 + 12*v0 + 13*w0 + 14*p0 + 15*q0 + 16*r0), Frad.X(), EPS);
    ASSERT_NEAR(conv * (21*u0 + 22*v0 + 23*w0 + 24*p0 + 25*q0 + 26*r0), Frad.Y(), EPS);
    ASSERT_NEAR(conv * (31*u0 + 32*v0 + 33*w0 + 34*p0 + 35*q0 + 36*r0), Frad.Z(), EPS);
    ASSERT_NEAR(conv * (41*u0 + 42*v0 + 43*w0 + 44*p0 + 45*q0 + 46*r0), Frad.K(), 10*EPS);
    ASSERT_NEAR(conv * (51*u0 + 52*v0 + 53*w0 + 54*p0 + 55*q0 + 56*r0), Frad.M(), 10*EPS);
    ASSERT_NEAR(conv * (61*u0 + 62*v0 + 63*w0 + 64*p0 + 65*q0 + 66*r0), Frad.N(), 10*EPS);
}
