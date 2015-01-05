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

