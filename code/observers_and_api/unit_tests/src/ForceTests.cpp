/*
 * ForceTests.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#include "ForceTester.hpp"
#include "ForceTests.hpp"
#include "generate_anthineas.hpp"
#include "GravityForceModel.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "STL_data.hpp"
#include "yaml_data.hpp"

#define EPS (1E-9)

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

ForceTests::ForceTests() : a(ssc::random_data_generator::DataGenerator(8222))
{
}

ForceTests::~ForceTests()
{
}

void ForceTests::SetUp()
{
}

void ForceTests::TearDown()
{
}

TEST_F(ForceTests, fast_hydrostatic_should_only_be_along_z)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<FastHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(-1026*0.5*9.81, F1.Z(),EPS) << "i = " << i;
        const auto F2 = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(-1026*0.5*9.81, F2.Z(),EPS) << "i = " << i;
        const double phi = a.random<double>().between(-PI,PI);
        const double theta = a.random<double>().between(-PI,PI);
        const double psi = a.random<double>().between(-PI,PI);
        const auto F3 = test.force_in_ned(0,0,0,phi,theta,psi);
        ASSERT_NEAR(-1026*0.5*9.81, F3.Z(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_fast_hydrostatic_should_only_be_along_z_for_anthineas)
{
    ForceTester test(test_data::oscillating_cube_example(), anthineas());

    test.add<FastHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI/4 + (PI/2)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(0, F1.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F1.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F1.Z()) << "i = " << i;
        const auto F2 = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(0, F2.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F2.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F2.Z()) << "i = " << i;
        const double phi = a.random<double>().between(-PI,PI);
        const double theta = a.random<double>().between(-PI,PI);
        const double psi = a.random<double>().between(-PI,PI);
        const auto F3 = test.force_in_ned(0,0,0,phi,theta,psi);
        ASSERT_NEAR(0, F3.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F3.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F3.Z()) << "i = " << i;
    }
}

TEST_F(ForceTests, rotation_around_z_should_not_change_anything_for_fast_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), anthineas());

    test.add<FastHydrostaticForceModel>();
    const size_t n = 100;
    const auto ref = test.force_in_ned(0,0,0,0,0,0);
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double psi = -PI/4 + (PI/2)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,0,0,psi);
        ASSERT_NEAR(ref.X(), F.X(),EPS) << "i = " << i;
        ASSERT_NEAR(ref.Y(), F.Y(),EPS) << "i = " << i;
        ASSERT_NEAR(ref.Z(), F.Z(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_no_torque_on_y_and_z_for_fast_hydrostatic_when_rotating_around_x)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<FastHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(0, F.M(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F.N(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_no_torque_on_x_and_z_for_fast_hydrostatic_when_rotating_around_y)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<FastHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(0, F.K(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F.N(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_no_torque_for_fast_hydrostatic_when_cube_is_at_45_deg_around_x)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<FastHydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/4,0,0);
    ASSERT_NEAR(0, F.K(),EPS);
    ASSERT_NEAR(0, F.M(),EPS);
    ASSERT_NEAR(0, F.N(),EPS);
}

TEST_F(ForceTests, norm_of_fast_torque_should_be_equal_for_opposite_rotations_of_the_cube)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<FastHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,0,angle,0);
        const auto F2 = test.force_in_ned(0,0,0,0,-angle,0);
        ASSERT_NEAR(std::sqrt(F1.K()*F1.K()+F1.M()*F1.M()+F1.N()*F1.N()), std::sqrt(F2.K()*F2.K()+F2.M()*F2.M()+F2.N()*F2.N()),EPS) << "i = " << i;
        const auto F3 = test.force_in_ned(0,0,0,angle,0,0);
        const auto F4 = test.force_in_ned(0,0,0,-angle,0,0);
        ASSERT_NEAR(std::sqrt(F3.K()*F3.K()+F3.M()*F3.M()+F3.N()*F3.N()), std::sqrt(F4.K()*F4.K()+F4.M()*F4.M()+F4.N()*F4.N()),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, force_should_be_correct_for_fast_hydrostatic_when_cube_is_at_45_deg_around_x_and_y)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<FastHydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/4,PI/4,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(-1026*0.5*9.81, F.Z(),EPS);
}

TEST_F(ForceTests, exact_hydrostatic_should_only_be_along_z)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<ExactHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(-1026*0.5*9.81, F1.Z(),EPS) << "i = " << i;
        const auto F2 = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(-1026*0.5*9.81, F2.Z(),EPS) << "i = " << i;
        const double phi = a.random<double>().between(-PI,PI);
        const double theta = a.random<double>().between(-PI,PI);
        const double psi = a.random<double>().between(-PI,PI);
        const auto F3 = test.force_in_ned(0,0,0,phi,theta,psi);
        ASSERT_NEAR(-1026*0.5*9.81, F3.Z(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_exact_hydrostatic_should_only_be_along_z_for_anthineas)
{
    ForceTester test(test_data::anthineas_exact_hydrostatic_test(), anthineas());

    test.add<ExactHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI/4 + (PI/2)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(0, F1.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F1.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F1.Z()) << "i = " << i;
        const auto F2 = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(0, F2.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F2.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F2.Z()) << "i = " << i;
        const double phi = a.random<double>().between(-PI,PI);
        const double theta = a.random<double>().between(-PI,PI);
        const double psi = a.random<double>().between(-PI,PI);
        const auto F3 = test.force_in_ned(0,0,0,phi,theta,psi);
        ASSERT_NEAR(0, F3.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F3.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F3.Z()) << "i = " << i;
    }
}

TEST_F(ForceTests, rotation_around_z_should_not_change_anything_for_exact_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), anthineas());

    test.add<ExactHydrostaticForceModel>();
    const size_t n = 100;
    const auto ref = test.force_in_ned(0,0,0,0,0,0);
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double psi = -PI/4 + (PI/2)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,0,0,psi);
        ASSERT_NEAR(ref.X(), F.X(),EPS) << "i = " << i;
        ASSERT_NEAR(ref.Y(), F.Y(),EPS) << "i = " << i;
        ASSERT_NEAR(ref.Z(), F.Z(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_no_torque_on_y_and_z_for_exact_hydrostatic_when_rotating_around_x)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<ExactHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(0, F.M(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F.N(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_no_torque_on_x_and_z_for_exact_hydrostatic_when_rotating_around_y)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<ExactHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(0, F.K(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F.N(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, DISABLED_no_torque_for_exact_hydrostatic_when_cube_is_at_45_deg_around_x)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<ExactHydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/4,0,0);
    ASSERT_NEAR(0, F.K(),EPS);
    ASSERT_NEAR(0, F.M(),EPS);
    ASSERT_NEAR(0, F.N(),EPS);
}

TEST_F(ForceTests, norm_of_exact_torque_should_be_equal_for_opposite_rotations_of_the_cube)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<ExactHydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,0,angle,0);
        const auto F2 = test.force_in_ned(0,0,0,0,-angle,0);
        ASSERT_NEAR(std::sqrt(F1.K()*F1.K()+F1.M()*F1.M()+F1.N()*F1.N()), std::sqrt(F2.K()*F2.K()+F2.M()*F2.M()+F2.N()*F2.N()),EPS) << "i = " << i;
        const auto F3 = test.force_in_ned(0,0,0,angle,0,0);
        const auto F4 = test.force_in_ned(0,0,0,-angle,0,0);
        ASSERT_NEAR(std::sqrt(F3.K()*F3.K()+F3.M()*F3.M()+F3.N()*F3.N()), std::sqrt(F4.K()*F4.K()+F4.M()*F4.M()+F4.N()*F4.N()),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, force_should_be_correct_for_exact_hydrostatic_when_cube_is_at_45_deg_around_x_and_y)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<ExactHydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/4,PI/4,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(-1026*0.5*9.81, F.Z(),EPS);
}

TEST_F(ForceTests, new_hydrostatic_should_only_be_along_z)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(-1026*0.5*9.81, F1.Z(),EPS) << "i = " << i;
        const auto F2 = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(-1026*0.5*9.81, F2.Z(),EPS) << "i = " << i;
        const double phi = a.random<double>().between(-PI,PI);
        const double theta = a.random<double>().between(-PI,PI);
        const double psi = a.random<double>().between(-PI,PI);
        const auto F3 = test.force_in_ned(0,0,0,phi,theta,psi);
        ASSERT_NEAR(-1026*0.5*9.81, F3.Z(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, new_hydrostatic_should_only_be_along_z_for_anthineas)
{
    ForceTester test(test_data::oscillating_cube_example(), anthineas());

    test.add<HydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI/4 + (PI/2)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(0, F1.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F1.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F1.Z()) << "i = " << i;
        const auto F2 = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(0, F2.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F2.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F2.Z()) << "i = " << i;
        const double phi = a.random<double>().between(-PI,PI);
        const double theta = a.random<double>().between(-PI,PI);
        const double psi = a.random<double>().between(-PI,PI);
        const auto F3 = test.force_in_ned(0,0,0,phi,theta,psi);
        ASSERT_NEAR(0, F3.X(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F3.Y(),EPS) << "i = " << i;
        ASSERT_GE(0, F3.Z()) << "i = " << i;
    }
}

TEST_F(ForceTests, rotation_around_z_should_not_change_anything_for_new_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), anthineas());

    test.add<HydrostaticForceModel>();
    const size_t n = 100;
    const auto ref = test.force_in_ned(0,0,0,0,0,0);
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double psi = -PI/4 + (PI/2)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,0,0,psi);
        ASSERT_NEAR(ref.X(), F.X(),EPS) << "i = " << i;
        ASSERT_NEAR(ref.Y(), F.Y(),EPS) << "i = " << i;
        ASSERT_NEAR(ref.Z(), F.Z(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, no_torque_on_y_and_z_for_new_hydrostatic_when_rotating_around_x)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,angle,0,0);
        ASSERT_NEAR(0, F.M(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F.N(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, no_torque_on_x_and_z_for_new_hydrostatic_when_rotating_around_y)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F = test.force_in_ned(0,0,0,0,angle,0);
        ASSERT_NEAR(0, F.K(),EPS) << "i = " << i;
        ASSERT_NEAR(0, F.N(),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, no_torque_for_new_hydrostatic_when_cube_is_at_45_deg_around_x)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/4,0,0);
    ASSERT_NEAR(0, F.K(),EPS);
    ASSERT_NEAR(0, F.M(),EPS);
    ASSERT_NEAR(0, F.N(),EPS);
}

TEST_F(ForceTests, norm_of_new_torque_should_be_equal_for_opposite_rotations_of_the_cube)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/std::max(1.,(double)n - 1);
        const auto F1 = test.force_in_ned(0,0,0,0,angle,0);
        const auto F2 = test.force_in_ned(0,0,0,0,-angle,0);
        ASSERT_NEAR(std::sqrt(F1.K()*F1.K()+F1.M()*F1.M()+F1.N()*F1.N()), std::sqrt(F2.K()*F2.K()+F2.M()*F2.M()+F2.N()*F2.N()),EPS) << "i = " << i;
        const auto F3 = test.force_in_ned(0,0,0,angle,0,0);
        const auto F4 = test.force_in_ned(0,0,0,-angle,0,0);
        ASSERT_NEAR(std::sqrt(F3.K()*F3.K()+F3.M()*F3.M()+F3.N()*F3.N()), std::sqrt(F4.K()*F4.K()+F4.M()*F4.M()+F4.N()*F4.N()),EPS) << "i = " << i;
    }
}

TEST_F(ForceTests, force_should_be_correct_for_new_hydrostatic_when_cube_is_at_45_deg_around_x_and_y)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/4,PI/4,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(-1026*0.5*9.81, F.Z(),EPS);
}

TEST_F(ForceTests, only_two_points_touch_the_surface_the_rest_is_above_for_new_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,-sqrt(2)/2,PI/4,0,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(0, F.Z(),EPS);
}

TEST_F(ForceTests, only_two_points_touch_the_surface_the_rest_is_under_for_new_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,sqrt(2)/2,PI/4,0,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(-1026*1*9.81, F.Z(),EPS);
}

TEST_F(ForceTests, one_facet_exactly_at_the_surface_and_the_rest_above_for_new_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,-0.5,0,0,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(0, F.Z(),EPS);
}

TEST_F(ForceTests, DISABLED_one_facet_exactly_at_the_surface_and_the_rest_under_for_new_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0.5,0,0,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(-1026*1*9.81, F.Z(),EPS);
}

