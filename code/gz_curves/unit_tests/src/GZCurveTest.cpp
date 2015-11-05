/*
 * GZCurveTest.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */


#include "GZCurveTest.hpp"
#include "GZCurve.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "STL_data.hpp"
#include "yaml_data.hpp"
#include "make_sim_for_GZ.hpp"
#include "generate_anthineas.hpp"
#include "stl_writer.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

Sim GZCurveTest::sim = GZ::make_sim(test_data::anthineas_hydrostatic_test("hydrostatic"), test_data::cube());

#define EPS 1E-10

GZCurveTest::GZCurveTest() : a(ssc::random_data_generator::DataGenerator(54522))
{
}

GZCurveTest::~GZCurveTest()
{
}

void GZCurveTest::SetUp()
{
}

void GZCurveTest::TearDown()
{
}

TEST_F(GZCurveTest, can_compute_angles_for_the_curve)
{
    const double dphi = 0.1;
    const double phi_max = 1;
    const std::vector<double> phi = GZ::Curve::get_phi(dphi, phi_max);
    ASSERT_EQ(21, phi.size());
    for (size_t i = 0 ; i < 21 ; ++i)
    {
        ASSERT_NEAR(-phi_max + ((double)i)*dphi, phi.at(i), 1E-10) << "i = " << i;
    }
}

TEST_F(GZCurveTest, can_compute_angles_for_the_curve_even_when_phi_is_not_a_multiple_of_dphi)
{
    const double dphi = 0.15;
    const double phi_max = 1;
    const std::vector<double> phi = GZ::Curve::get_phi(dphi, phi_max);
    ASSERT_EQ(15, phi.size());
    ASSERT_DOUBLE_EQ(-phi_max, phi.front());
    ASSERT_DOUBLE_EQ(phi_max, phi.back());
    for (size_t i = 1 ; i < 13 ; ++i)
    {
        ASSERT_NEAR(dphi, phi.at(i+1)-phi.at(i), EPS);
    }
}

TEST_F(GZCurveTest, can_compute_angles_for_the_curve_even_when_phi_equals_dphi)
{
    const double dphi = 1;
    const double phi_max = 1;
    const std::vector<double> phi = GZ::Curve::get_phi(dphi, phi_max);
    ASSERT_EQ(3, phi.size());
    ASSERT_DOUBLE_EQ(-phi_max, phi.at(0));
    ASSERT_DOUBLE_EQ(0, phi.at(1));
    ASSERT_DOUBLE_EQ(phi_max, phi.at(2));
}

TEST_F(GZCurveTest, should_throw_if_dphi_is_not_strictly_positive)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double dphi = a.random<double>().no().greater_than(0);
        const double phi_max = a.random<double>().greater_than(0);
        ASSERT_THROW(GZ::Curve::get_phi(dphi, phi_max), InvalidInputException);
    }
}

TEST_F(GZCurveTest, should_throw_if_phi_max_is_not_strictly_positive)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double dphi = a.random<double>().greater_than(0);
        const double phi_max = a.random<double>().no().greater_than(0);
        ASSERT_THROW(GZ::Curve::get_phi(dphi, phi_max), InvalidInputException);
    }
}

TEST_F(GZCurveTest, should_throw_if_phi_max_lower_than_dphi)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double dphi = a.random<double>().greater_than(0);
        const double phi_max = a.random<double>().between(0,dphi);
        ASSERT_THROW(GZ::Curve::get_phi(dphi, phi_max), InvalidInputException);
    }
}

TEST_F(GZCurveTest, can_calculate_zeq_for_phi_0)
{
    const Sim sim = GZ::make_sim(test_data::oscillating_cube_example(), test_data::cube());
    const GZ::Curve calculate(sim);
    ASSERT_NEAR(1000./1026.-0.5, calculate.zeq(0, 0),EPS);
}

TEST_F(GZCurveTest, can_calculate_GZ_for_phi_0)
{
    const Sim sim = GZ::make_sim(test_data::oscillating_cube_example(), test_data::cube());
    const GZ::Curve calculate(sim);
    ASSERT_NEAR(0, calculate.gz(0),EPS);
}

TEST_F(GZCurveTest, LONG_validate_gz_against_python_code)
{
    const Sim sim = GZ::make_sim(test_data::anthineas_damping(), write_stl(anthineas()));
    const GZ::Curve calculate(sim);

    ASSERT_SMALL_RELATIVE_ERROR(-7.78E-001, calculate.gz(-70*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-7.87E-001, calculate.gz(-65*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-7.86E-001, calculate.gz(-60*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-7.70E-001, calculate.gz(-55*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-7.39E-001, calculate.gz(-50*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-6.85E-001, calculate.gz(-45*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-6.04E-001, calculate.gz(-40*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-5.13E-001, calculate.gz(-35*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-4.24E-001, calculate.gz(-30*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-3.39E-001, calculate.gz(-25*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-2.63E-001, calculate.gz(-20*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-1.93E-001, calculate.gz(-15*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-1.27E-001, calculate.gz(-10*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(-6.27E-002, calculate.gz(-5*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(4.10E-004, calculate.gz(0), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(6.20E-002, calculate.gz(5*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(1.26E-001, calculate.gz(10*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(1.92E-001, calculate.gz(15*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(2.63E-001, calculate.gz(20*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(3.39E-001, calculate.gz(25*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(4.24E-001, calculate.gz(30*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(5.12E-001, calculate.gz(35*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(6.04E-001, calculate.gz(40*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(6.85E-001, calculate.gz(45*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(7.39E-001, calculate.gz(50*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(7.71E-001, calculate.gz(55*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(7.86E-001, calculate.gz(60*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(7.88E-001, calculate.gz(65*PI/180.), 0.05);
    ASSERT_SMALL_RELATIVE_ERROR(7.78E-001, calculate.gz(70*PI/180.), 0.05);
}
