/*
 * force_parsersTest.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "force_parsersTest.hpp"
#include "force_parsers.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define DEG2RAD (PI/180.)

force_parsersTest::force_parsersTest() : a(ssc::random_data_generator::DataGenerator(86556))
{
}

force_parsersTest::~force_parsersTest()
{
}

void force_parsersTest::SetUp()
{
}

void force_parsersTest::TearDown()
{
}

TEST_F(force_parsersTest, wageningen)
{
    const YamlWageningen w = parse_wageningen(test_data::wageningen());
    ASSERT_DOUBLE_EQ(0.5, w.blade_area_ratio);
    ASSERT_EQ("port side propeller", w.name);
    ASSERT_EQ(3, w.number_of_blades);
    ASSERT_DOUBLE_EQ(0, w.position_of_propeller_frame.angle.phi);
    ASSERT_DOUBLE_EQ(-10*DEG2RAD, w.position_of_propeller_frame.angle.theta);
    ASSERT_DOUBLE_EQ(-1*DEG2RAD, w.position_of_propeller_frame.angle.psi);
    ASSERT_DOUBLE_EQ(-4, w.position_of_propeller_frame.coordinates.x);
    ASSERT_DOUBLE_EQ(-2, w.position_of_propeller_frame.coordinates.y);
    ASSERT_DOUBLE_EQ(2, w.position_of_propeller_frame.coordinates.z);
    ASSERT_EQ("mesh(body 1)", w.position_of_propeller_frame.frame);
    ASSERT_DOUBLE_EQ(1, w.relative_rotative_efficiency);
    ASSERT_TRUE(w.rotating_clockwise);
    ASSERT_DOUBLE_EQ(0.7, w.thrust_deduction_factor);
    ASSERT_DOUBLE_EQ(0.9, w.wake_coefficient);
    ASSERT_DOUBLE_EQ(2, w.diameter);
}

TEST_F(force_parsersTest, resistance_curves)
{
    const YamlResistanceCurve r = parse_resistance_curve(test_data::resistance_curve());
    ASSERT_EQ(8, r.R.size());
    ASSERT_EQ(8, r.Va.size());

    const double knot = 0.51444444444444444;
    ASSERT_DOUBLE_EQ(0, r.Va[0]);
    ASSERT_DOUBLE_EQ(1*knot, r.Va[1]);
    ASSERT_DOUBLE_EQ(2*knot, r.Va[2]);
    ASSERT_DOUBLE_EQ(3*knot, r.Va[3]);
    ASSERT_DOUBLE_EQ(4*knot, r.Va[4]);
    ASSERT_DOUBLE_EQ(5*knot, r.Va[5]);
    ASSERT_DOUBLE_EQ(15*knot, r.Va[6]);
    ASSERT_DOUBLE_EQ(20*knot, r.Va[7]);

    ASSERT_DOUBLE_EQ(0, r.R[0]);
    ASSERT_DOUBLE_EQ(1E6, r.R[1]);
    ASSERT_DOUBLE_EQ(4E6, r.R[2]);
    ASSERT_DOUBLE_EQ(9E6, r.R[3]);
    ASSERT_DOUBLE_EQ(16E6, r.R[4]);
    ASSERT_DOUBLE_EQ(25E6, r.R[5]);
    ASSERT_DOUBLE_EQ(225E6, r.R[6]);
    ASSERT_DOUBLE_EQ(400E6, r.R[7]);
}

TEST_F(force_parsersTest, radiation_damping)
{
    const YamlRadiationDamping r = parse_radiation_damping(test_data::radiation_damping());
    ASSERT_EQ("anthineas.hdb", r.hdb_filename);
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

TEST_F(force_parsersTest, diffraction)
{
    const YamlDiffraction r = parse_diffraction(test_data::diffraction());
    ASSERT_EQ("anthineas.hdb", r.hdb_filename);
    ASSERT_EQ(0.696, r.calculation_point.x);
    ASSERT_EQ(0, r.calculation_point.y);
    ASSERT_EQ(1.418, r.calculation_point.z);
    ASSERT_TRUE(r.mirror);
}
