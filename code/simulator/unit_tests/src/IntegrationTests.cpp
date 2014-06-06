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
