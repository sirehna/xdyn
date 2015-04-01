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
#include "stl_writer.hpp"
#include "TriMeshTestData.hpp"

#define EPS (1E-8)

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define ANTHINEAS_VOLUME 601.726637766
#define ANTHINEAS_SURFACE 495.51109419742073214

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

TEST_F(ForceTests, LONG_fast_hydrostatic_should_only_be_along_z)
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

TEST_F(ForceTests, LONG_rotation_around_z_should_not_change_anything_for_fast_hydrostatic)
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

TEST_F(ForceTests, LONG_norm_of_fast_torque_should_be_equal_for_opposite_rotations_of_the_cube)
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

TEST_F(ForceTests, LONG_exact_hydrostatic_should_only_be_along_z)
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

TEST_F(ForceTests, LONG_rotation_around_z_should_not_change_anything_for_exact_hydrostatic)
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

TEST_F(ForceTests, LONG_norm_of_exact_torque_should_be_equal_for_opposite_rotations_of_the_cube)
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

TEST_F(ForceTests, LONG_new_hydrostatic_should_only_be_along_z)
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

TEST_F(ForceTests, LONG_new_hydrostatic_should_only_be_along_z_for_anthineas)
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

TEST_F(ForceTests, LONG_rotation_around_z_should_not_change_anything_for_new_hydrostatic)
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

TEST_F(ForceTests, LONG_norm_of_new_torque_should_be_equal_for_opposite_rotations_of_the_cube)
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

TEST_F(ForceTests, one_facet_exactly_at_the_surface_and_the_rest_under_for_new_hydrostatic)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0.5,0,0,0);
    ASSERT_NEAR(0, F.X(),EPS);
    ASSERT_NEAR(0, F.Y(),EPS);
    ASSERT_NEAR(-1026*1*9.81, F.Z(),EPS);
}

TEST_F(ForceTests, can_compute_torque_for_a_cube_rotated_on_phi)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/3,0,0);
    ASSERT_NEAR(-1026*0.5*9.81, F.Z(),EPS);
    EXPECT_NEAR(0, F.M(),EPS);
    EXPECT_NEAR(-1026*0.5*9.81*1/36., F.K(),EPS);
}

TEST_F(ForceTests, can_compute_torque_for_a_cube_rotated_on_theta)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,0,PI/3,0);
    ASSERT_NEAR(-1026*0.5*9.81, F.Z(),EPS);
    EXPECT_NEAR(0, F.K(),EPS);
    EXPECT_NEAR(-1026*0.5*9.81*1/36., F.M(),EPS);
}

TEST_F(ForceTests, LONG_validation_of_anthineas_volume_against_VTK)
{
    ForceTester test(test_data::anthineas_damping(), write_stl(anthineas()));
    ASSERT_NEAR(ANTHINEAS_VOLUME, test.emerged_volume(0,0,-100,0,0,0), 1E-4);
    ASSERT_NEAR(ANTHINEAS_VOLUME, test.immersed_volume(0,0,100,0,0,0), 1E-4);
}

TEST_F(ForceTests, validation_of_L_volume_against_VTK)
{
    ForceTester test(test_data::L_config(), L());
    const double z0 = -2.5/3;
    ASSERT_NEAR(0, test.immersed_volume(0,0,z0,0,0,0), 1E-10);
    ASSERT_NEAR(0.5, test.immersed_volume(0,0,z0+0.25,0,0,0), 1E-10);
    ASSERT_NEAR(1, test.immersed_volume(0,0,z0+0.5,0,0,0), 1E-10);
    ASSERT_NEAR(2, test.immersed_volume(0,0,z0+1,0,0,0), 1E-10);
    ASSERT_NEAR(2.5, test.immersed_volume(0,0,z0+1.5,0,0,0), 1E-10);
    ASSERT_NEAR(3, test.immersed_volume(0,0,z0+2,0,0,0), 1E-10);
    ASSERT_NEAR(3, test.emerged_volume(0,0,z0,0,0,0), 1E-10);
    ASSERT_NEAR(2.5, test.emerged_volume(0,0,z0+0.25,0,0,0), 1E-10);
    ASSERT_NEAR(2, test.emerged_volume(0,0,z0+0.5,0,0,0), 1E-10);
    ASSERT_NEAR(1, test.emerged_volume(0,0,z0+1,0,0,0), 1E-10);
    ASSERT_NEAR(0.5, test.emerged_volume(0,0,z0+1.5,0,0,0), 1E-10);
    ASSERT_NEAR(0, test.emerged_volume(0,0,z0+2,0,0,0), 1E-10);
}

TEST_F(ForceTests, LONG_immersed_and_emerged_surfaces_are_correct)
{
    ForceTester test(test_data::anthineas_damping(), write_stl(anthineas()));
    double immersed_surface = test.immersed_surface(0,0,1,-PI/2,0,0);
    double emerged_surface = test.emerged_surface(0,0,1,-PI/2,0,0);
    ASSERT_NEAR(495.51109419742073214, immersed_surface+emerged_surface, 1E-10);
    immersed_surface = test.immersed_surface(0,0,-2.4,PI/4,0,0);
    emerged_surface = test.emerged_surface(0,0,-2.4,PI/4,0,0);
    ASSERT_NEAR(495.51109419742073214, immersed_surface+emerged_surface, 1E-10);
}

TEST_F(ForceTests, DISABLED_center_of_buoyancy_L)
{
    ForceTester test(test_data::L_config(), L());
    test.add<HydrostaticForceModel>();
    const double z0 = -2.5/3;
    const double eps = 1E-10;
    for (double z = 0.1 ; z < 1 ; z+=0.1)
    {
        const auto centre_of_buoyancy_in_ned = test.center_of_buoyancy_in_ned_frame(+0.5,2.5/3,z+z0,0,0,0);
        ASSERT_NEAR(0.5, (double)centre_of_buoyancy_in_ned(0), eps) << "z[ned]=" << z << ", B = " << centre_of_buoyancy_in_ned.transpose();
        ASSERT_NEAR(1, (double)centre_of_buoyancy_in_ned(1), eps) << "z[ned]=" << z << ", B = " << centre_of_buoyancy_in_ned.transpose();
        ASSERT_NEAR(z/2, (double)centre_of_buoyancy_in_ned(2), eps) << "z[ned]=" << z << ", B = " << centre_of_buoyancy_in_ned.transpose();
    }
    for (double z = 0 ; z < 1 ; z+=0.1)
    {
        const auto centre_of_buoyancy_in_ned = test.center_of_buoyancy_in_ned_frame(+0.5,2.5/3,z+1+z0,0,0,0);
        ASSERT_NEAR(0.5, (double)centre_of_buoyancy_in_ned(0), eps) << "z[ned]=" << z+1 << ", B = " << centre_of_buoyancy_in_ned.transpose();
        ASSERT_NEAR((2+z/2)/(2+z), (double)centre_of_buoyancy_in_ned(1), eps) << "z[ned]=" << z+1 << ", B = " << centre_of_buoyancy_in_ned.transpose();
        ASSERT_NEAR((1+z)-(1+z+z*z/2)/(2+z), (double)centre_of_buoyancy_in_ned(2), eps) << "z[ned]=" << z+1 << ", B = " << centre_of_buoyancy_in_ned.transpose();
    }
    const auto B = test.center_of_buoyancy_in_ned_frame(+0.5,2.5/3,z0,PI/2,0,0);
    ASSERT_NEAR(0.5, (double)B(0), eps) << "B = " << B.transpose();
    ASSERT_NEAR(2.5/3, (double)B(1), eps) << "B = " << B.transpose();;
    ASSERT_NEAR(2.5/3, (double)B(2), eps) << "B = " << B.transpose();;
}

TEST_F(ForceTests, DISABLED_center_of_buoyancy_cube)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());
    test.add<HydrostaticForceModel>();
    const double eps = 1E-10;
    EPoint B = test.center_of_buoyancy_in_ned_frame(0,0,0,PI/4,0,0);
    ASSERT_NEAR(0, (double)B(0), eps) << "B = " << B.transpose();
    ASSERT_NEAR(0, (double)B(1), eps) << "B = " << B.transpose();
    ASSERT_NEAR(sqrt(2)/6, (double)B(2), eps) << "B = " << B.transpose();
    B = test.center_of_buoyancy_in_ned_frame(0,0,0,PI/2,0,0);
    ASSERT_NEAR(0, (double)B(0), eps) << "B = " << B.transpose();
    ASSERT_NEAR(0, (double)B(1), eps) << "B = " << B.transpose();
    ASSERT_NEAR(sqrt(2)/6, (double)B(2), eps) << "B = " << B.transpose();
}

TEST_F(ForceTests, DISABLED_LONG_bug_detected_in_GZ)
{
    const double eps = 1.0103e-12;
    ForceTester test(test_data::anthineas_damping(), write_stl(anthineas()));
    test.immersed_volume(0,0,-eps/2,-PI/2,0,0);
    const double V1_immersed = test.immersed_volume(0,0,-eps/2,-PI/2,0,0);
    const double V1_emerged = test.immersed_volume(0,0,-eps/2,-PI/2,0,0);
    const double V2_immersed = test.immersed_volume(0,0,+eps/2,-PI/2,0,0);
    const double V2_emerged = test.immersed_volume(0,0,+eps/2,-PI/2,0,0);

    ASSERT_NEAR(ANTHINEAS_VOLUME/2, V1_immersed, 1E-1);
    ASSERT_NEAR(ANTHINEAS_VOLUME/2, V1_emerged, 1E-1);
    ASSERT_NEAR(ANTHINEAS_VOLUME/2, V2_immersed, 1E-1);
    ASSERT_NEAR(ANTHINEAS_VOLUME/2, V2_emerged, 1E-1);

    ASSERT_NEAR(ANTHINEAS_VOLUME, V1_immersed+V1_emerged, 1E-1);
    ASSERT_NEAR(V1_immersed, V1_emerged, 1E-1);
    ASSERT_NEAR(V2_immersed, V2_emerged, 1E-1);

    ASSERT_NEAR(ANTHINEAS_VOLUME, V2_immersed+V2_emerged, 1E-1);
    ASSERT_NEAR(V1_immersed, V2_immersed+V2_emerged, 1E-1);
    ASSERT_NEAR(V1_emerged, V2_emerged, 1);
}
