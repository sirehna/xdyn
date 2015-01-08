/*
 * SimCsvObserverTest.cpp
 *
 *  Created on: Aug 28, 2014
 *      Author: cady
 */

#include "SimCsvObserverTest.hpp"
#include "SimCsvObserver.hpp"
#include "simulator_api.hpp"
#include "StateMacros.hpp" // For StateType
#include <ssc/solver.hpp>
#include "TriMeshTestData.hpp"
#include "yaml_data.hpp"

SimCsvObserverTest::SimCsvObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

SimCsvObserverTest::~SimCsvObserverTest()
{
}

void SimCsvObserverTest::SetUp()
{
}

void SimCsvObserverTest::TearDown()
{
}

TEST_F(SimCsvObserverTest, can_simulate_system_with_no_environment)
{
    const double dt = 1E-1;
    const double tend = 10;
    std::stringstream simulation_stream;
    std::stringstream wave_stream;
    SimCsvObserver observer(simulation_stream, wave_stream);
    auto sys = get_system(test_data::falling_ball_example());
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
}

TEST_F(SimCsvObserverTest, can_simulate_system_with_no_wave_output)
{
    const double dt = 1E-1;
    const double tend = 10;
    std::stringstream simulation_stream;
    std::stringstream wave_stream;
    SimCsvObserver observer(simulation_stream, wave_stream);
    auto sys = get_system(test_data::anthineas_fast_hydrostatic_test(), unit_cube());
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
}

TEST_F(SimCsvObserverTest, can_simulate_environment_with_no_system_for_two_time_steps)
{
    const double dt = 1E-1;
    const double tend = 0.1;
    std::stringstream simulation_stream;
    std::stringstream wave_stream;
    SimCsvObserver observer(simulation_stream, wave_stream);
    auto sys = get_system(test_data::waves());
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    std::stringstream expected;

    expected << "waves:\n"
             << "  x: [1,2,3,4,5,1,2,3,4,5]\n"
             << "  y: [1,1,1,1,1,2,2,2,2,2]\n"
             << "  timesteps:\n"
             << "    - t: 0\n"
             << "      z: [-4.60386,-4.60388,-4.6039,-4.60392,-4.60393,-4.6553,-4.65531,-4.65533,-4.65535,-4.65537]\n"
             << "    - t: 0.1\n"
             << "      z: [-4.52551,-4.52553,-4.52554,-4.52556,-4.52557,-4.58458,-4.5846,-4.58462,-4.58463,-4.58465]\n";
    ASSERT_EQ(expected.str(), wave_stream.str());
}

TEST_F(SimCsvObserverTest, can_simulate_both_environment_and_system_for_two_time_steps)
{
    const double dt = 1E-1;
    const double tend = dt;
    std::stringstream simulation_stream;
    std::stringstream wave_stream;
    SimCsvObserver observer(simulation_stream, wave_stream);
    auto sys = get_system(test_data::cube_in_waves(), unit_cube());
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
    std::stringstream expected;
    expected << "waves:\n"
             << "  timesteps:\n"
             << "    - t: 0\n"
             << "      x: [1,2,3,4,5,1,2,3,4,5]\n"
             << "      y: [1,1,1,1,1,2,2,2,2,2]\n"
             << "      z: [-2.78529,-2.7853,-2.7853,-2.78531,-2.78531,-2.83606,-2.83607,-2.83608,-2.83608,-2.83609]\n"
             << "    - t: 0.1\n"
             << "      x: [0.9,1.9,2.9,3.9,4.9,0.9,1.9,2.9,3.9,4.9]\n"
             << "      y: [0.8,0.8,0.8,0.8,0.8,1.8,1.8,1.8,1.8,1.8]\n"
             << "      z: [-2.71298,-2.71298,-2.71299,-2.71299,-2.713,-2.76903,-2.76903,-2.76904,-2.76904,-2.76905]\n";
    ASSERT_EQ(expected.str(), wave_stream.str());
}
