/*
 * ForceTests.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#include <boost/optional/optional_io.hpp>

#include "ForceTester.hpp"
#include "ForceTests.hpp"
#include "generate_test_ship.hpp"
#include "GravityForceModel.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "stl_data.hpp"
#include "yaml_data.hpp"
#include "hdb_data.hpp"
#include "stl_writer.hpp"
#include "TriMeshTestData.hpp"
#include "GMForceModel.hpp"
#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"

#include <ssc/macros.hpp>

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

TEST_F(ForceTests, DISABLED_fast_hydrostatic_should_only_be_along_z_for_test_ship)
{
    ForceTester test(test_data::oscillating_cube_example(), test_ship());

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
    ForceTester test(test_data::oscillating_cube_example(), test_ship());

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

TEST_F(ForceTests, DISABLED_exact_hydrostatic_should_only_be_along_z_for_test_ship)
{
    ForceTester test(test_data::test_ship_exact_hydrostatic_test(), test_ship());

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
    ForceTester test(test_data::oscillating_cube_example(), test_ship());

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

TEST_F(ForceTests, LONG_new_hydrostatic_should_only_be_along_z_for_test_ship)
{
    ForceTester test(test_data::oscillating_cube_example(), test_ship());

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
    ForceTester test(test_data::oscillating_cube_example(), test_ship());

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

TEST_F(ForceTests, LONG_validation_of_test_ship_volume_against_VTK)
{
    ForceTester test(test_data::test_ship_damping(), write_stl(test_ship()));
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
    ForceTester test(test_data::test_ship_damping(), write_stl(test_ship()));
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
    ForceTester test(test_data::test_ship_damping(), write_stl(test_ship()));
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

TEST_F(ForceTests, bug_3004)
{
    ForceTester test(test_data::bug_3004(), test_data::cube());
    test.add<GMForceModel>("model: gm \n"
                           "name of hydrostatic force model: non-linear hydrostatic (fast)\n"
                           "roll step: {value: 1, unit: degree}");
    const auto gm = test.gm(0,0,-5e-07,0,0,0);
    ASSERT_TRUE(gm.is_initialized());
    ASSERT_FALSE(std::isnan(gm.get()));
}

TEST_F(ForceTests, hydrostatic_plus_froude_krylov)
{
    ForceTester test(test_data::oscillating_cube_example(), test_data::cube());

    test.add<HydrostaticForceModel>();
    const auto F = test.force_in_ned(0,0,0,PI/3,0,0);
    ASSERT_NEAR(-1026*0.5*9.81, F.Z(),EPS);
    EXPECT_NEAR(0, F.M(),EPS);
    EXPECT_NEAR(-1026*0.5*9.81*1/36., F.K(),EPS);
}

DiffractionForceModel ForceTests::get_diffraction_force_model(const YamlModel& waves, const std::string& diffraction_yaml, const std::string& hdb_file_contents) const
{
    const std::string yaml = test_data::test_ship_waves_test();
    const std::string stl = test_data::single_facet();
    auto input = SimulatorYamlParser(yaml).parse();
    YamlBody body = input.bodies.front();
    body.controlled_forces.clear();
    body.external_forces.clear();

    input.bodies[0] = body;
    input.environment.clear();
    input.environment.push_back(waves);

    const auto sys = get_system(check_input_yaml(input), stl, 0);
    const auto env = sys.get_env();
    const ssc::kinematics::Transform T(ssc::kinematics::Point("NED",0,0,0), "Anthineas");
    env.k->add(T);
    const YamlDiffraction data = DiffractionForceModel::parse(diffraction_yaml);
    return DiffractionForceModel(data, body.name, env, hdb_file_contents);
}

YamlModel ForceTests::get_regular_wave(const double propagation_angle_in_ned_frame_in_degrees, const double Hs_in_meters, const double Tp_in_seconds) const
{
    YamlModel wave;
    wave.model = "waves";
    std::stringstream ss;
    ss << "discretization:\n"
       << "   n: 10\n"
       << "   omega min: {value: 0.1, unit: rad/s}\n"
       << "   omega max: {value: 6, unit: rad/s}\n"
       << "   energy fraction: 0.999\n"
       << "spectra:\n"
       << "  - model: airy\n"
       << "    depth: {value: 100, unit: m}\n"
       << "    seed of the random data generator: none\n"
       << "    stretching:\n"
       << "      delta: 1\n"
       << "      h: {unit: m, value: 0}\n"
       << "    directional spreading:\n"
       << "       type: dirac\n"
       << "       waves propagating to: {value: " << propagation_angle_in_ned_frame_in_degrees << ", unit: deg}\n"
       << "    spectral density:\n"
       << "       type: dirac\n"
       << "       Hs: {value: " << Hs_in_meters << ", unit: m}\n"
       << "       omega0: {value: " << 2*PI/Tp_in_seconds << ", unit: rad/s}";
    wave.yaml = ss.str();
    return wave;
}

std::string ForceTests::get_diffraction_conf(const double x, const double y, const double z) const
{
    std::stringstream ss;
    ss << "model: diffraction\n"
           << "hdb: test_ship.hdb\n"
           << "calculation point in body frame:\n"
           << "    x: {value: " << x << ", unit: m}\n"
           << "    y: {value: " << y << ", unit: m}\n"
           << "    z: {value: " << z << ", unit: m}\n"
           << "mirror for 180 to 360: true\n";
    return ss.str();
}

BodyStates get_whole_body_state_with_psi_equal_to(const double psi);
BodyStates get_whole_body_state_with_psi_equal_to(const double psi)
{
    const std::vector<double> s = get_states(0,0,0,0,0,psi*PI/180);
    BodyStates states;
    states.x.record(0,s.at(0));
    states.y.record(0,s.at(1));
    states.z.record(0,s.at(2));
    states.qr.record(0,s.at(9));
    states.qi.record(0,s.at(10));
    states.qj.record(0,s.at(11));
    states.qk.record(0,s.at(12));
    states.convention.order_by = "angle";
    states.convention.convention.push_back("z");
    states.convention.convention.push_back("y'");
    states.convention.convention.push_back("x''");
    states.name = "Anthineas";
    return states;
}

BodyStates get_whole_body_state(const double x, const double y, const double z, const double phi, const double theta, const double psi);
BodyStates get_whole_body_state(const double x, const double y, const double z, const double phi, const double theta, const double psi)
{
    const std::vector<double> s = get_states(x,y,z,phi*PI/180,theta*PI/180,psi*PI/180);
    BodyStates states;
    states.x.record(0,s.at(0));
    states.y.record(0,s.at(1));
    states.z.record(0,s.at(2));
    states.qr.record(0,s.at(9));
    states.qi.record(0,s.at(10));
    states.qj.record(0,s.at(11));
    states.qk.record(0,s.at(12));
    states.convention.order_by = "angle";
    states.convention.convention.push_back("z");
    states.convention.convention.push_back("y'");
    states.convention.convention.push_back("x''");
    states.name = "Anthineas";
    return states;
}

TEST_F(ForceTests, bug_3210_no_interpolation_in_incidence_and_no_incidence_no_interpolation_in_period_no_transport)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(0, 2, 4);
    const std::string config_such_that_rao_point_is_zero = get_diffraction_conf(0,0,0);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_zero, test_data::bug_3210());

    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 0;
    const auto tau = F(states, t);
    // Line in HDB corresponding to module (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_MOD_001]   0.000000):
    // 5.084407E+04  0.000000E+00  3.997774E+05  0.000000E+00  3.209051E+07  0.000000E+00
    const std::vector<double> module = {5.084407E+04,  0.000000E+00,  3.997774E+05,  0.000000E+00,  3.209051E+07,  0.000000E+00};
    // Line in HDB corresponding to phase (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_PH_001]   0.000000)
    // -1.135123E+00  1.570796E+00 -8.389206E-01  1.570796E+00 -8.356066E-01  1.570796E+00
    const std::vector<double> phase = {-1.135123E+00,  1.570796E+00,  -8.389206E-01,  1.570796E+00,  -8.356066E-01,  1.570796E+00};

    ASSERT_DOUBLE_EQ( module[0]*sin(-phase[0]), tau.X());
    ASSERT_DOUBLE_EQ(-module[1]*sin(-phase[1]), tau.Y()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[2]*sin(-phase[2]), tau.Z()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ( module[3]*sin(-phase[3]), tau.K());
    ASSERT_DOUBLE_EQ(-module[4]*sin(-phase[4]), tau.M()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[5]*sin(-phase[5]), tau.N()); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_no_interpolation_in_incidence_but_incidence_30_no_interpolation_in_period_no_transport)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(-30, 2, 4);
    const std::string config_such_that_rao_point_is_zero = get_diffraction_conf(0,0,0);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 0;
    const auto tau = F(states, t);
    // Line in HDB corresponding to module (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_MOD_001]   30.00000):
    // 8.567009E+04  3.284304E+05  5.820626E+05  1.460688E+06  3.140660E+07  2.301511E+07
    const std::vector<double> module = {8.567009E+04,  3.284304E+05,  5.820626E+05,  1.460688E+06,  3.140660E+07,  2.301511E+07};
    // Line in HDB corresponding to phase (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_PH_001]   30.00000)
    // 2.077326E+00 -5.459499E-01  1.525810E+00 -6.670656E-01  1.375271E+00 -5.846877E-01
    const std::vector<double> phase = {2.077326E+00, -5.459499E-01,  1.525810E+00, -6.670656E-01,  1.375271E+00, -5.846877E-01};
    ASSERT_DOUBLE_EQ( module[0]*sin(-phase[0]), tau.X());
    ASSERT_DOUBLE_EQ(-module[1]*sin(-phase[1]), tau.Y()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[2]*sin(-phase[2]), tau.Z()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ( module[3]*sin(-phase[3]), tau.K());
    ASSERT_DOUBLE_EQ(-module[4]*sin(-phase[4]), tau.M()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[5]*sin(-phase[5]), tau.N()); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_interpolation_in_incidence_no_interpolation_in_period_no_transport)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(-15, 2, 4);
    const std::string config_such_that_rao_point_is_zero = get_diffraction_conf(0,0,0);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 0;
    const auto tau = F(states, t);
    // Interpolate at 15° using the following lines of the HDB file (see two previous tests):
    // 5.084407E+04  0.000000E+00  3.997774E+05  0.000000E+00  3.209051E+07  0.000000E+00
    // 8.567009E+04  3.284304E+05  5.820626E+05  1.460688E+06  3.140660E+07  2.301511E+07
    const std::vector<double> module =
        { (5.084407E+04+8.567009E+04)/2
        , (0.000000E+00+3.284304E+05)/2
        , (3.997774E+05+5.820626E+05)/2
        , (0.000000E+00+1.460688E+06)/2
        , (3.209051E+07+3.140660E+07)/2
        , (0.000000E+00+2.301511E+07)/2
        };
    // Interpolate at 15° using the following lines of the HDB file (see two previous tests):
    // -1.135123E+00  1.570796E+00 -8.389206E-01  1.570796E+00 -8.356066E-01  1.570796E+00
    // 2.077326E+00 -5.459499E-01  1.525810E+00 -6.670656E-01  1.375271E+00 -5.846877E-01
    const std::vector<double> phase =
        { (-1.135123E+00+2.077326E+00)/2
        , (1.570796E+00-5.459499E-01)/2
        , (-8.389206E-01+ 1.525810E+00)/2
        , (1.570796E+00-6.670656E-01)/2
        , (-8.356066E-01+ 1.375271E+00)/2
        , (1.570796E+00-5.846877E-01)/2
        };
    ASSERT_DOUBLE_EQ( module[0]*sin(-phase[0]), tau.X());
    ASSERT_DOUBLE_EQ(-module[1]*sin(-phase[1]), tau.Y()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[2]*sin(-phase[2]), tau.Z()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ( module[3]*sin(-phase[3]), tau.K());
    ASSERT_DOUBLE_EQ(-module[4]*sin(-phase[4]), tau.M()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[5]*sin(-phase[5]), tau.N()); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_no_interpolation_in_incidence_interpolation_in_period_no_transport)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_10 = get_regular_wave(-30, 2, 10);
    const std::string config_such_that_rao_point_is_zero = get_diffraction_conf(0,0,0);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_10, config_such_that_rao_point_is_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 0;
    const auto tau = F(states, t);
    // Interpolate at Tp=10s using the following lines of the HDB file (see two previous tests):
    // 8.567009E+04  3.284304E+05  5.820626E+05  1.460688E+06  3.140660E+07  2.301511E+07
    // 3.275670E+03  3.409648E+04  3.283568E+05  1.685674E+04  2.255748E+06  4.594584E+05
    const std::vector<double> module =
        { (8.567009E+04*(64.-10) + 3.275670E+03*(10-4))/(64.-4.)
        , (3.284304E+05*(64.-10) + 3.409648E+04*(10-4))/(64.-4.)
        , (5.820626E+05*(64.-10) + 3.283568E+05*(10-4))/(64.-4.)
        , (1.460688E+06*(64.-10) + 1.685674E+04*(10-4))/(64.-4.)
        , (3.140660E+07*(64.-10) + 2.255748E+06*(10-4))/(64.-4.)
        , (2.301511E+07*(64.-10) + 4.594584E+05*(10-4))/(64.-4.)
        };
    // Interpolate at Tp=10s using the following lines of the HDB file (see two previous tests):
    // 2.077326E+00 -5.459499E-01  1.525810E+00 -6.670656E-01  1.375271E+00 -5.846877E-01
    // 2.028876E+00 -3.130008E+00  1.636047E+00 -1.350179E-01  1.768062E+00 -3.053413E+00
    const std::vector<double> phase =
        { ( 2.077326E+00*(64.-10.) + 2.028876E+00*(10-4))/(64.-4.)
        , (-5.459499E-01*(64.-10.)  -3.130008E+00*(10-4))/(64.-4.)
        , ( 1.525810E+00*(64.-10.) + 1.636047E+00*(10-4))/(64.-4.)
        , (-6.670656E-01*(64.-10.)  -1.350179E-01*(10-4))/(64.-4.)
        , ( 1.375271E+00*(64.-10.) + 1.768062E+00*(10-4))/(64.-4.)
        , (-5.846877E-01*(64.-10.)  -3.053413E+00*(10-4))/(64.-4.)
        };
    const double eps = 1E-6;
    ASSERT_SMALL_RELATIVE_ERROR( module[0]*sin(-phase[0]), tau.X(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[1]*sin(-phase[1]), tau.Y(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[2]*sin(-phase[2]), tau.Z(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR( module[3]*sin(-phase[3]), tau.K(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[4]*sin(-phase[4]), tau.M(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[5]*sin(-phase[5]), tau.N(), eps); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_no_interpolation_in_incidence_no_interpolation_in_period_transport)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(0, 2, 4);
    const std::string config_such_that_rao_point_is_not_zero = get_diffraction_conf(1,2,3);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_not_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 0;
    const auto tau = F(states, t);
    // Line in HDB corresponding to module (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_MOD_001]   0.000000):
    // 5.084407E+04  0.000000E+00  3.997774E+05  0.000000E+00  3.209051E+07  0.000000E+00
    const std::vector<double> module = {5.084407E+04,  0.000000E+00,  3.997774E+05,  0.000000E+00,  3.209051E+07,  0.000000E+00};
    // Line in HDB corresponding to phase (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_PH_001]   0.000000)
    // -1.135123E+00  1.570796E+00 -8.389206E-01  1.570796E+00 -8.356066E-01  1.570796E+00
    const std::vector<double> phase = {-1.135123E+00,  1.570796E+00,  -8.389206E-01,  1.570796E+00,  -8.356066E-01,  1.570796E+00};
    const double k = (2*PI/4)*(2*PI/4)/9.81;
    const double eps = 1E-5;
    ASSERT_SMALL_RELATIVE_ERROR( module[0]*sin(k-phase[0]), tau.X(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[1]*sin(k-phase[1]), tau.Y(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[2]*sin(k-phase[2]), tau.Z(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR( module[3]*sin(k-phase[3])+2*tau.Z()-3*tau.Y(), tau.K(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[4]*sin(k-phase[4])+3*tau.X()-tau.Z(), tau.M(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[5]*sin(k-phase[5])+tau.Y()-2*tau.X(), tau.N(), eps); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_no_interpolation_in_incidence_no_interpolation_in_period_transport_with_non_zero_t)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(0, 2, 4);
    const std::string config_such_that_rao_point_is_not_zero = get_diffraction_conf(1,2,3);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_not_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 5;
    const auto tau = F(states, t);
    // Line in HDB corresponding to module (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_MOD_001]   0.000000):
    // 5.084407E+04  0.000000E+00  3.997774E+05  0.000000E+00  3.209051E+07  0.000000E+00
    const std::vector<double> module = {5.084407E+04,  0.000000E+00,  3.997774E+05,  0.000000E+00,  3.209051E+07,  0.000000E+00};
    // Line in HDB corresponding to phase (first line of section [DIFFRACTION_FORCES_AND_MOMENTS]/[INCIDENCE_EFM_PH_001]   0.000000)
    // -1.135123E+00  1.570796E+00 -8.389206E-01  1.570796E+00 -8.356066E-01  1.570796E+00
    const std::vector<double> phase = {-1.135123E+00,  1.570796E+00,  -8.389206E-01,  1.570796E+00,  -8.356066E-01,  1.570796E+00};
    const double k = (2*PI/4)*(2*PI/4)/9.81;
    const double eps = 1E-4;
    ASSERT_SMALL_RELATIVE_ERROR( module[0]*sin(k-2.5*PI-phase[0]), tau.X(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[1]*sin(k-2.5*PI-phase[1]), tau.Y(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[2]*sin(k-2.5*PI-phase[2]), tau.Z(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR( module[3]*sin(k-2.5*PI-phase[3])+2*tau.Z()-3*tau.Y(), tau.K(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[4]*sin(k-2.5*PI-phase[4])+3*tau.X()-tau.Z(), tau.M(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[5]*sin(k-2.5*PI-phase[5])+tau.Y()-2*tau.X(), tau.N(), eps); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_no_interpolation_in_incidence_interpolation_in_period_transport_with_non_zero_t)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_10 = get_regular_wave(30, 2, 10);
    const std::string config_such_that_rao_point_is_not_zero = get_diffraction_conf(1,2,3);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_10, config_such_that_rao_point_is_not_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(0);
    const double t = 3;
    const auto tau = F(states, t);
    // Interpolate at Tp=10s using the following lines of the HDB file (see two previous tests):
    // 8.567009E+04  3.284304E+05  5.820626E+05  1.460688E+06  3.140660E+07  2.301511E+07
    // 3.275670E+03  3.409648E+04  3.283568E+05  1.685674E+04  2.255748E+06  4.594584E+05
    const std::vector<double> module =
        { (8.567009E+04*(64.-10) + 3.275670E+03*(10-4))/(64.-4.)
        , (3.284304E+05*(64.-10) + 3.409648E+04*(10-4))/(64.-4.)
        , (5.820626E+05*(64.-10) + 3.283568E+05*(10-4))/(64.-4.)
        , (1.460688E+06*(64.-10) + 1.685674E+04*(10-4))/(64.-4.)
        , (3.140660E+07*(64.-10) + 2.255748E+06*(10-4))/(64.-4.)
        , (2.301511E+07*(64.-10) + 4.594584E+05*(10-4))/(64.-4.)
        };
    // Interpolate at Tp=10s using the following lines of the HDB file (see two previous tests):
    // 2.077326E+00 -5.459499E-01  1.525810E+00 -6.670656E-01  1.375271E+00 -5.846877E-01
    // 2.028876E+00 -3.130008E+00  1.636047E+00 -1.350179E-01  1.768062E+00 -3.053413E+00
    const std::vector<double> phase =
        { ( 2.077326E+00*(64.-10.) + 2.028876E+00*(10-4))/(64.-4.)
        , (-5.459499E-01*(64.-10.)  -3.130008E+00*(10-4))/(64.-4.)
        , ( 1.525810E+00*(64.-10.) + 1.636047E+00*(10-4))/(64.-4.)
        , (-6.670656E-01*(64.-10.)  -1.350179E-01*(10-4))/(64.-4.)
        , ( 1.375271E+00*(64.-10.) + 1.768062E+00*(10-4))/(64.-4.)
        , (-5.846877E-01*(64.-10.)  -3.053413E+00*(10-4))/(64.-4.)
        };
    const double k = (2*PI/10)*(2*PI/10)/9.81;
    const double eps = 1E-4;
    // Careful of the signs: cf. bug 3227
    ASSERT_SMALL_RELATIVE_ERROR( module[0]*sin(k*(cos(PI/6)+2*sin(PI/6))-0.6*PI-phase[0]), tau.X(), eps);
    ASSERT_SMALL_RELATIVE_ERROR( module[1]*sin(k*(cos(PI/6)+2*sin(PI/6))-0.6*PI-phase[1]), tau.Y(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[2]*sin(k*(cos(PI/6)+2*sin(PI/6))-0.6*PI-phase[2]), tau.Z(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR(-module[3]*sin(k*(cos(PI/6)+2*sin(PI/6))-0.6*PI-phase[3])+2*tau.Z()-3*tau.Y(), tau.K(), eps);
    ASSERT_SMALL_RELATIVE_ERROR(-module[4]*sin(k*(cos(PI/6)+2*sin(PI/6))-0.6*PI-phase[4])+3*tau.X()-tau.Z(), tau.M(), eps); // Z is down for X-DYN and up for AQUA+
    ASSERT_SMALL_RELATIVE_ERROR( module[5]*sin(k*(cos(PI/6)+2*sin(PI/6))-0.6*PI-phase[5])+tau.Y()-2*tau.X(), tau.N(), eps); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3210_interpolation_with_non_zero_psi_in_incidence_no_interpolation_in_period_no_transport)
{
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(-3, 2, 4);
    const std::string config_such_that_rao_point_is_zero = get_diffraction_conf(0,0,0);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_zero, test_data::bug_3210());
    const auto states = get_whole_body_state_with_psi_equal_to(12);
    const double t = 0;
    const auto tau = F(states, t);
    // Interpolate at 15° using the following lines of the HDB file (see two previous tests):
    // 5.084407E+04  0.000000E+00  3.997774E+05  0.000000E+00  3.209051E+07  0.000000E+00
    // 8.567009E+04  3.284304E+05  5.820626E+05  1.460688E+06  3.140660E+07  2.301511E+07
    const std::vector<double> module =
        { (5.084407E+04+8.567009E+04)/2
        , (0.000000E+00+3.284304E+05)/2
        , (3.997774E+05+5.820626E+05)/2
        , (0.000000E+00+1.460688E+06)/2
        , (3.209051E+07+3.140660E+07)/2
        , (0.000000E+00+2.301511E+07)/2
        };
    // Interpolate at 15° using the following lines of the HDB file (see two previous tests):
    // -1.135123E+00  1.570796E+00 -8.389206E-01  1.570796E+00 -8.356066E-01  1.570796E+00
    // 2.077326E+00 -5.459499E-01  1.525810E+00 -6.670656E-01  1.375271E+00 -5.846877E-01
    const std::vector<double> phase =
        { (-1.135123E+00+2.077326E+00)/2
        , (1.570796E+00-5.459499E-01)/2
        , (-8.389206E-01+ 1.525810E+00)/2
        , (1.570796E+00-6.670656E-01)/2
        , (-8.356066E-01+ 1.375271E+00)/2
        , (1.570796E+00-5.846877E-01)/2
        };
    ASSERT_DOUBLE_EQ( module[0]*sin(-phase[0]), tau.X());
    ASSERT_DOUBLE_EQ(-module[1]*sin(-phase[1]), tau.Y()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[2]*sin(-phase[2]), tau.Z()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ( module[3]*sin(-phase[3]), tau.K());
    ASSERT_DOUBLE_EQ(-module[4]*sin(-phase[4]), tau.M()); // Z is down for X-DYN and up for AQUA+
    ASSERT_DOUBLE_EQ(-module[5]*sin(-phase[5]), tau.N()); // Z is down for X-DYN and up for AQUA+
}

TEST_F(ForceTests, bug_3239_reference_frame_is_incorrect)
{
    const double propagation_angle_in_ned_frame_in_degrees = 180;
    const double Hs_in_meters = 0.0485;
    const double Tp_in_seconds = 4;
    const YamlModel regular_waves_Hs_1_propagating_to_north_Tp_equals_4 = get_regular_wave(propagation_angle_in_ned_frame_in_degrees, Hs_in_meters, Tp_in_seconds);
    const std::string config_such_that_rao_point_is_zero = get_diffraction_conf(23,29,31);
    const DiffractionForceModel F = get_diffraction_force_model(regular_waves_Hs_1_propagating_to_north_Tp_equals_4, config_such_that_rao_point_is_zero, test_data::bug_3210());
    const auto states = get_whole_body_state(0,0,0,0,0,0);
    const double t = 0;
    const auto tau = F(states, t);
    ASSERT_EQ("Anthineas", tau.get_frame());
}
