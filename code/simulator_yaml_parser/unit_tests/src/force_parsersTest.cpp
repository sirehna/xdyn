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

TEST_F(force_parsersTest, gravity)
{
    const YamlGravity p = parse_gravity("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity");
    ASSERT_DOUBLE_EQ(9.81, p.g);
}

TEST_F(force_parsersTest, hydrostatic)
{
    const double rho = parse_hydrostatic("rho:\n  unit: kg/m^3\n  value: 1024\nmodel: hydrostatic");
    ASSERT_DOUBLE_EQ(1024, rho);
}

TEST_F(force_parsersTest, damping)
{
    const std::string yaml = "        damping matrix at the center of gravity projected in the body frame:\n"
                             "            frame: body 1\n"
                             "            row 1: [  2,   3,   5,   7,  11,  13]\n"
                             "            row 2: [ 17,  19,  23,  29,  31,  37]\n"
                             "            row 3: [ 41,  43,  47,  53,  59,  61]\n"
                             "            row 4: [ 67,  71,  73,  79,  83,  89]\n"
                             "            row 5: [ 97, 101, 103, 107, 109, 113]\n"
                             "            row 6: [127, 131, 137, 139, 149, 151]\n";
    const Eigen::Matrix<double,6,6> D = parse_quadratic_damping(yaml);
    ASSERT_DOUBLE_EQ(2.,(double)D(0,0));
    ASSERT_DOUBLE_EQ(3.,(double)D(0,1));
    ASSERT_DOUBLE_EQ(5.,(double)D(0,2));
    ASSERT_DOUBLE_EQ(7.,(double)D(0,3));
    ASSERT_DOUBLE_EQ(11.,(double)D(0,4));
    ASSERT_DOUBLE_EQ(13.,(double)D(0,5));
    ASSERT_DOUBLE_EQ(17.,(double)D(1,0));
    ASSERT_DOUBLE_EQ(19.,(double)D(1,1));
    ASSERT_DOUBLE_EQ(23.,(double)D(1,2));
    ASSERT_DOUBLE_EQ(29.,(double)D(1,3));
    ASSERT_DOUBLE_EQ(31.,(double)D(1,4));
    ASSERT_DOUBLE_EQ(37.,(double)D(1,5));
    ASSERT_DOUBLE_EQ(41.,(double)D(2,0));
    ASSERT_DOUBLE_EQ(43.,(double)D(2,1));
    ASSERT_DOUBLE_EQ(47.,(double)D(2,2));
    ASSERT_DOUBLE_EQ(53.,(double)D(2,3));
    ASSERT_DOUBLE_EQ(59.,(double)D(2,4));
    ASSERT_DOUBLE_EQ(61.,(double)D(2,5));
    ASSERT_DOUBLE_EQ(67.,(double)D(3,0));
    ASSERT_DOUBLE_EQ(71.,(double)D(3,1));
    ASSERT_DOUBLE_EQ(73.,(double)D(3,2));
    ASSERT_DOUBLE_EQ(79.,(double)D(3,3));
    ASSERT_DOUBLE_EQ(83.,(double)D(3,4));
    ASSERT_DOUBLE_EQ(89.,(double)D(3,5));
    ASSERT_DOUBLE_EQ(97.,(double)D(4,0));
    ASSERT_DOUBLE_EQ(101.,(double)D(4,1));
    ASSERT_DOUBLE_EQ(103.,(double)D(4,2));
    ASSERT_DOUBLE_EQ(107.,(double)D(4,3));
    ASSERT_DOUBLE_EQ(109.,(double)D(4,4));
    ASSERT_DOUBLE_EQ(113.,(double)D(4,5));
    ASSERT_DOUBLE_EQ(127.,(double)D(5,0));
    ASSERT_DOUBLE_EQ(131.,(double)D(5,1));
    ASSERT_DOUBLE_EQ(137.,(double)D(5,2));
    ASSERT_DOUBLE_EQ(139.,(double)D(5,3));
    ASSERT_DOUBLE_EQ(149.,(double)D(5,4));
    ASSERT_DOUBLE_EQ(151.,(double)D(5,5));
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
