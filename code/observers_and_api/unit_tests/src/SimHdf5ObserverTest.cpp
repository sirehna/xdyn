/*
 * SimHdf5ObserverTest.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: jacquenot
 */

#include "SimHdf5ObserverTest.hpp"
#include "SimHdf5Observer.hpp"
#include "simulator_api.hpp"
#include "StateMacros.hpp" // For StateType
#include <ssc/solver.hpp>
#include "yaml_data.hpp"
#include "TriMeshTestData.hpp"

SimHdf5ObserverTest::SimHdf5ObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

SimHdf5ObserverTest::~SimHdf5ObserverTest()
{
}

void SimHdf5ObserverTest::SetUp()
{
}

void SimHdf5ObserverTest::TearDown()
{
}

TEST_F(SimHdf5ObserverTest, can_simulate_system_with_no_environment)
{
    const double dt = 1E-1;
    const double tend = 10;
    const std::string fileName("can_simulate_system_with_no_environment.h5");
    const std::string datasetName("simu0");
    auto sys = get_system(test_data::falling_ball_example());
    {
        SimHdf5Observer observer(fileName, datasetName, sys);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(SimHdf5ObserverTest, can_simulate_system_with_no_wave_output)
{
    const double dt = 1E-1;
    const double tend = 10;
    const std::string fileName("can_simulate_system_with_no_wave_output.h5");
    const std::string datasetName("simu0");
    auto sys = get_system(test_data::anthineas_fast_hydrostatic_test(), unit_cube());
    {
        SimHdf5Observer observer(fileName, datasetName, sys);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(SimHdf5ObserverTest, can_retrieve_the_output_wave_mesh_size)
{
    const auto ms = get_system(test_data::cube_in_waves(), unit_cube()).get_waves_mesh_size();
    ASSERT_EQ(5,ms.first);
    ASSERT_EQ(2,ms.second);
}

TEST_F(SimHdf5ObserverTest, should_be_able_to_create_an_observer_for_states_efforts_and_waves)
{
    const std::string fileName("should_be_able_to_create_an_observer_for_states_efforts_and_waves.h5");
    const std::string datasetName("simu0");
    auto sys = get_system(test_data::cube_in_waves(), unit_cube());
    ASSERT_EQ(5,sys.get_waves_mesh_size().first);
    ASSERT_EQ(2,sys.get_waves_mesh_size().second);
    {
        SimHdf5Observer observer(fileName, datasetName, sys);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(SimHdf5ObserverTest, can_simulate_both_environment_and_system_for_two_time_steps)
{
    const double dt = 1E-1;
    const double tend = dt;
    const std::string fileName("can_simulate_both_environment_and_system_for_two_time_steps.h5");
    const std::string datasetName("simu0");
    auto sys = get_system(test_data::cube_in_waves(), unit_cube());
    {
        SimHdf5Observer observer(fileName, datasetName, sys);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    }
    //EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(SimHdf5ObserverTest, can_simulate_both_environment_and_system_for_several_time_steps)
{
    const double dt = 1E-1;
    const double tend = 10*dt;
    const std::string fileName("can_simulate_both_environment_and_system_for_several_time_steps.h5");
    const std::string datasetName("simu0");
    auto sys = get_system(test_data::cube_in_waves(), unit_cube());
    {
        SimHdf5Observer observer(fileName, datasetName, sys);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    }
    //EXPECT_EQ(0,remove(fileName.c_str()));
}
