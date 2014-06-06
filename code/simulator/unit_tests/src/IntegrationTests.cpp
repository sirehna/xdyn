/*
 * IntegrationTests.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: cady
 */

#include "IntegrationTests.hpp"
#include "yaml_data.hpp"
#include "DsMapObserver.hpp"
#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"
#include "DsSystem.hpp"
#include "DsSolve.hpp"

IntegrationTests::IntegrationTests() : a(DataGenerator(1212))
{
}

IntegrationTests::~IntegrationTests()
{
}

void IntegrationTests::SetUp()
{
}

void IntegrationTests::TearDown()
{
}

TEST_F(IntegrationTests, yaml_data_for_falling_ball_should_be_valid)
{
    check_input_yaml(SimulatorYamlParser(test_data::falling_ball_example()).parse());
}

TEST_F(IntegrationTests, simulator_does_not_crash_for_falling_ball)
{
    DataSource ds = make_ds(test_data::falling_ball_example(),1,"euler");
    DsMapObserver observer;
    integrate(ds, 0, 10, observer);
}
