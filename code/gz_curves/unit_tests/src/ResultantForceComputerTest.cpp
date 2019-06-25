/*
 * ResultantForceComputerTest.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include <set>

#include "make_sim_for_GZ.hpp"
#include "ResultantForceComputerTest.hpp"
#include "ResultantForceComputer.hpp"
#include "SurfaceElevationInterface.hpp"
#include "GZTypes.hpp"
#include "stl_data.hpp"
#include "yaml_data.hpp"

#define EPS (1E-10)
#define BIG_EPS (1E-5)

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

Sim ResultantForceComputerTest::sim = GZ::make_sim(test_data::oscillating_cube_example(), test_data::cube());

ResultantForceComputerTest::ResultantForceComputerTest() : a(ssc::random_data_generator::DataGenerator(87878899))
{
}

ResultantForceComputerTest::~ResultantForceComputerTest()
{
}

void ResultantForceComputerTest::SetUp()
{
}

void ResultantForceComputerTest::TearDown()
{
}

TEST_F(ResultantForceComputerTest, sim_only_contains_gravity_and_hydrostatic_forces)
{
    sim.reset_history();
    const auto s = GZ::make_sim(test_data::stable_cube_example(), test_data::cube());

    std::map<std::string,std::vector<ForcePtr> > forces = s.get_forces();
    std::set<std::string> force_models;
    for (auto force:forces["cube"])
    {
        force_models.insert(force->get_name());
    }
    ASSERT_EQ(2, force_models.size());
    const bool has_gravity = force_models.find("gravity") != force_models.end();
    const bool has_hydrostatic = force_models.find("non-linear hydrostatic (fast)") != force_models.end();
    ASSERT_TRUE(has_gravity);
    ASSERT_TRUE(has_hydrostatic);
}

TEST_F(ResultantForceComputerTest, sim_only_contains_default_surface_elevation)
{
    sim.reset_history();
    const auto s = GZ::make_sim(test_data::cube_in_waves(), test_data::cube());
    EnvironmentAndFrames env = s.get_env();
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const std::vector<double> x{a.random<double>().between(-1000, 1000)};
        const std::vector<double> y{a.random<double>().between(-1000, 1000)};
        const double t = a.random<double>().between(0,1000);
        ASSERT_DOUBLE_EQ(0, env.w->get_and_check_wave_height(x,y,t).at(0));
    }
}

TEST_F(ResultantForceComputerTest, can_compute_resultant_force_for_a_cube)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F0 = sum_of_forces.resultant(GZ::State(0,0,0)).state;
    ASSERT_NEAR(1000*9.81-1026*0.5*9.81, F0(0),EPS);
    ASSERT_NEAR(0, F0(1),EPS);
    ASSERT_NEAR(0, F0(2),EPS);
    const auto F1 = sum_of_forces.resultant(GZ::State(2,0,0)).state;
    ASSERT_NEAR(1000*9.81-1026*1.*9.81, F1(0),EPS);
    ASSERT_NEAR(0, F1(1),EPS);
    ASSERT_NEAR(0, F1(2),EPS);
}

TEST_F(ResultantForceComputerTest, DISABLED_can_compute_torque_for_a_cube_rotated_on_phi)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces.resultant(GZ::State(0,PI/3,0)).state;
    ASSERT_NEAR((1000*9.81-1026*0.5*9.81)*cos(PI/3), F(0),EPS);
    ASSERT_NEAR(-1026*9.81*0.5*(2./9.*cos(PI/3.)-0.096225044864937658851*sin(PI/3.)), F(1),EPS);
    ASSERT_NEAR(0, F(2),EPS);
}

TEST_F(ResultantForceComputerTest, DISABLED_can_compute_torque_for_a_cube_rotated_on_theta)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces.resultant(GZ::State(0,0,PI/3)).state;
    ASSERT_NEAR((1000*9.81-1026*0.5*9.81)*cos(PI/3), F(0),EPS);
    EXPECT_NEAR(0, F(1),EPS);
    EXPECT_NEAR(-1026*9.81*0.5*(2./9.*cos(PI/3.)-0.096225044864937658851*sin(PI/3.)), F(2),EPS);
}

TEST_F(ResultantForceComputerTest, DISABLED_can_compute_torque_for_a_cube_rotated_by_a_quarter)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces.resultant(GZ::State(0,PI/4,0)).state;
    ASSERT_NEAR((1000*9.81-1026*0.5*9.81)*cos(PI/4), F(0),EPS);
    ASSERT_NEAR(0, F(1),EPS);
    ASSERT_NEAR(0, F(2),EPS);
}

TEST_F(ResultantForceComputerTest, can_compute_torque_for_a_cube_at_the_surface)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces.resultant(GZ::State(-0.5,0,0)).state;
    ASSERT_NEAR(1000*9.81, F(0),EPS);
    ASSERT_NEAR(0, F(1),EPS);
    ASSERT_NEAR(0, F(2),EPS);
}

TEST_F(ResultantForceComputerTest, can_compute_resultant_for_half_immersed_cube)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces.resultant(GZ::State(0,0,0)).state;
    ASSERT_NEAR(1000*9.81-1026*0.5*9.81, F(0),EPS);
    ASSERT_NEAR(0, F(1),EPS);
    ASSERT_NEAR(0, F(2),EPS);
}

TEST_F(ResultantForceComputerTest, LONG_can_compute_resultant_for_any_angle)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(GZ::make_sim(test_data::oscillating_cube_example(), test_data::cube()));
    const size_t n = 100;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double angle = -PI + (2*PI)*(double)(i)/(std::max(1.,(double)n - 1));
        const auto F1 = sum_of_forces.resultant(GZ::State(0,angle,0)).state;
        ASSERT_NEAR((1000*9.81-1026*0.5*9.81), F1(0),EPS) << "angle = " << angle;
        const auto F2 = sum_of_forces.resultant(GZ::State(0,0,angle)).state;
        ASSERT_NEAR((1000*9.81-1026*0.5*9.81), F2(0),EPS) << "angle = " << angle;
    }

}

TEST_F(ResultantForceComputerTest, DISABLED_can_compute_centre_of_buoyancy_for_a_cube_rotated_on_phi)
{
    sim.reset_history();
    GZ::ResultantForceComputer sum_of_forces(sim);
    const double phi = PI/3;
    const auto gz = sum_of_forces.resultant(GZ::State(0,phi,0)).gz;
    ASSERT_NEAR(1./18., gz, EPS);
}

TEST_F(ResultantForceComputerTest, can_get_zmin_and_zmax_for_phi_0)
{
    sim.reset_history();
    GZ::ResultantForceComputer cube(sim);
    const GZ::MinMax z = cube.get_zmin_zmax(0);
    ASSERT_DOUBLE_EQ(-0.5, z.min);
    ASSERT_DOUBLE_EQ(0.5, z.max);
}

TEST_F(ResultantForceComputerTest, can_get_zmin_and_zmax_for_phi_pi_4)
{
    sim.reset_history();
    GZ::ResultantForceComputer cube(sim);
    const GZ::MinMax z = cube.get_zmin_zmax(PI/4);
    ASSERT_DOUBLE_EQ(-sqrt(2)/2., z.min);
    ASSERT_DOUBLE_EQ(sqrt(2)/2., z.max);
}

TEST_F(ResultantForceComputerTest, can_get_zmin_and_zmax_for_phi_pi_3)
{
    sim.reset_history();
    GZ::ResultantForceComputer cube(sim);
    const GZ::MinMax z = cube.get_zmin_zmax(PI/3);
    ASSERT_DOUBLE_EQ(-(1+sqrt(3))/4, z.min);
    ASSERT_DOUBLE_EQ((1+sqrt(3))/4, z.max);
}

TEST_F(ResultantForceComputerTest, can_get_zmin_and_zmax_for_phi_pi_2)
{
    sim.reset_history();
    GZ::ResultantForceComputer cube(sim);
    const GZ::MinMax z = cube.get_zmin_zmax(PI/2);
    ASSERT_DOUBLE_EQ(-0.5, z.min);
    ASSERT_DOUBLE_EQ(0.5, z.max);
}

TEST_F(ResultantForceComputerTest, can_get_zmin_and_zmax_for_phi_minus_pi_6)
{
    sim.reset_history();
    GZ::ResultantForceComputer cube(sim);
    const GZ::MinMax z = cube.get_zmin_zmax(-PI/6);
    ASSERT_DOUBLE_EQ(-(1+sqrt(3))/4, z.min);
    ASSERT_DOUBLE_EQ((1+sqrt(3))/4, z.max);
}

TEST_F(ResultantForceComputerTest, DISABLED_can_compute_K)
{
    sim.reset_history();
    GZ::ResultantForceComputer cube(sim);
    const auto K =  cube.K(Eigen::Vector3d(0,0,0));

    ASSERT_NEAR(0, (double)K(0,1), BIG_EPS);
    ASSERT_NEAR(0, (double)K(1,0), BIG_EPS);

    ASSERT_NEAR(-10065.060000670201, (double)K(0,0), BIG_EPS);
    ASSERT_NEAR(  419.3774999700085, (double)K(1,1), BIG_EPS);
}
