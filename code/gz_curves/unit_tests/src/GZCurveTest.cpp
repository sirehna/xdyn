/*
 * GZCurveTest.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */


#include "GZCurveTest.hpp"
#include "GZCurve.hpp"
#include "GZException.hpp"
#include "STL_data.hpp"
#include "yaml_data.hpp"
#include "make_sim_for_GZ.hpp"

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
        ASSERT_THROW(GZ::Curve::get_phi(dphi, phi_max), GZException);
    }
}

TEST_F(GZCurveTest, should_throw_if_phi_max_is_not_strictly_positive)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double dphi = a.random<double>().greater_than(0);
        const double phi_max = a.random<double>().no().greater_than(0);
        ASSERT_THROW(GZ::Curve::get_phi(dphi, phi_max), GZException);
    }
}

TEST_F(GZCurveTest, should_throw_if_phi_max_lower_than_dphi)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double dphi = a.random<double>().greater_than(0);
        const double phi_max = a.random<double>().between(0,dphi);
        ASSERT_THROW(GZ::Curve::get_phi(dphi, phi_max), GZException);
    }
}
