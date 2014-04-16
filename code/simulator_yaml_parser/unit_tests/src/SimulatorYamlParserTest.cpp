/*
 * SimulatorYamlParserTest.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */


#include "SimulatorYamlParserTest.hpp"
#include "YamlSimulatorInput.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"

SimulatorYamlParserTest::SimulatorYamlParserTest() : a(DataGenerator(1212))
{
}

SimulatorYamlParserTest::~SimulatorYamlParserTest()
{
}

void SimulatorYamlParserTest::SetUp()
{
}

void SimulatorYamlParserTest::TearDown()
{
}

TEST_F(SimulatorYamlParserTest, can_parse_rotations)
{
    const YamlRotation rot = SimulatorYamlParser(test_data::hydrostatic_test()).parse().rotations;
    ASSERT_EQ("angle", rot.order_by);
    ASSERT_EQ(3, rot.convention.size());
    ASSERT_EQ("x", rot.convention.at(0));
    ASSERT_EQ("y'", rot.convention.at(1));
    ASSERT_EQ("z''", rot.convention.at(2));
}

TEST_F(SimulatorYamlParserTest, can_parse_models)
{
    const YamlModel mod1 = SimulatorYamlParser(test_data::hydrostatic_test()).parse().environment.at(0);
    ASSERT_EQ("no waves", mod1.model);
    ASSERT_EQ("frame: NED\nmodel: no waves\nz:\n  unit: m\n  value: 0", mod1.yaml);
    const YamlModel mod2 = SimulatorYamlParser(test_data::hydrostatic_test()).parse().bodies.at(0).external_forces.at(0);
    ASSERT_EQ("gravity", mod2.model);
    ASSERT_EQ("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity", mod2.yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_environment)
{
    const std::vector<YamlModel> env = SimulatorYamlParser(test_data::hydrostatic_test()).parse().environment;
    ASSERT_EQ(2, env.size());
    ASSERT_EQ("no waves", env.at(0).model);
    ASSERT_EQ("no wind", env.at(1).model);
}

TEST_F(SimulatorYamlParserTest, can_parse_bodies)
{
    const YamlSimulatorInput input = SimulatorYamlParser(test_data::hydrostatic_test()).parse();
    ASSERT_EQ(1, input.bodies.size());
    ASSERT_EQ("body 1", input.bodies.at(0).name);
    ASSERT_EQ("anthineas.stl", input.bodies.at(0).mesh);
    ASSERT_EQ(2, input.bodies.at(0).external_forces.size());
}
