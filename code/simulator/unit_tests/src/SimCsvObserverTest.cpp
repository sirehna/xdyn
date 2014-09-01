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
#include "steppers.hpp"
#include "TriMeshTestData.hpp"
#include "yaml_data.hpp"

SimCsvObserverTest::SimCsvObserverTest() : a(DataGenerator(546545))
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
    quicksolve<EulerStepper>(sys, 0, tend, dt, observer);
}

TEST_F(SimCsvObserverTest, can_simulate_system_with_no_wave_output)
{
    const double dt = 1E-1;
    const double tend = 10;
    std::stringstream simulation_stream;
    std::stringstream wave_stream;
    SimCsvObserver observer(simulation_stream, wave_stream);
    auto sys = get_system(test_data::anthineas_hydrostatic_test(), unit_cube());
    quicksolve<EulerStepper>(sys, 0, tend, dt, observer);
}

TEST_F(SimCsvObserverTest, can_simulate_environment_with_no_system)
{
    const double dt = 1E-1;
    const double tend = 0.1;
    std::stringstream simulation_stream;
    std::stringstream wave_stream;
    SimCsvObserver observer(simulation_stream, wave_stream);
    auto sys = get_system(test_data::waves());
    quicksolve<EulerStepper>(sys, 0, tend, dt, observer);
    std::stringstream expected;
    expected << "waves:\n"
             << "  x: [1,2,3,4,5,1,2,3,4,5]\n"
             << "  y: [1,1,1,1,1,2,2,2,2,2]\n"
             << "  timesteps:\n"
             << "    - t: 0\n"
             << "      z: [-49.098,-49.098,-49.098,-49.098,-49.0979,-48.6016,-48.6016,-48.6016,-48.6016,-48.6016]\n"
             << "    - t: 0.1\n"
             << "      z: [-49.6365,-49.6365,-49.6364,-49.6364,-49.6364,-49.1101,-49.1101,-49.1101,-49.1101,-49.1101]\n";
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
    quicksolve<EulerStepper>(sys, 0, tend, dt, observer);
    std::stringstream expected;

    expected << "waves:\n"
             << "  timesteps:\n"
             << "    - t: 0\n"
             << "      x: [1,2,3,4,5,1,2,3,4,5]\n"
             << "      y: [1,1,1,1,1,2,2,2,2,2]\n"
             << "      z: [-30.838,-30.838,-30.838,-30.8379,-30.8379,-30.3429,-30.3428,-30.3428,-30.3428,-30.3428]\n"
             << "    - t: 0.1\n"
             << "      x: [0.9,1.9,2.9,3.9,4.9,0.9,1.9,2.9,3.9,4.9]\n"
             << "      y: [0.8,0.8,0.8,0.8,0.8,1.8,1.8,1.8,1.8,1.8]\n"
             << "      z: [-31.4784,-31.4784,-31.4784,-31.4784,-31.4784,-30.8921,-30.8921,-30.8921,-30.8921,-30.8921]\n";
    ASSERT_EQ(expected.str(), wave_stream.str());
}
