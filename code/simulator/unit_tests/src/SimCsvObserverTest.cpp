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
    auto sys = get_system(test_data::anthineas_hydrostatic_test(), unit_cube());
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observer);
}

TEST_F(SimCsvObserverTest, can_simulate_environment_with_no_system)
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
             << "      z: [3.7318,3.73178,3.73177,3.73176,3.73174,3.64687,3.64686,3.64684,3.64683,3.64682]\n"
             << "    - t: 0.1\n"
             << "      z: [3.82541,3.8254,3.82538,3.82537,3.82535,3.72587,3.72586,3.72584,3.72583,3.72581]\n";
    ASSERT_EQ(expected.str(), wave_stream.str());
}

TEST_F(SimCsvObserverTest, can_simulate_both_environment_and_system)
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
             << "      z: [2.89043,2.89043,2.89042,2.89041,2.8904,2.80581,2.8058,2.8058,2.80579,2.80578]\n"
             << "    - t: 0.1\n"
             << "      x: [0.9,1.9,2.9,3.9,4.9,0.9,1.9,2.9,3.9,4.9]\n"
             << "      y: [0.8,0.8,0.8,0.8,0.8,1.8,1.8,1.8,1.8,1.8]\n"
             << "      z: [3.00309,3.00308,3.00307,3.00307,3.00306,2.8922,2.89219,2.89218,2.89217,2.89216]\n";
    ASSERT_EQ(expected.str(), wave_stream.str());
}
